.. _boundary_communication:

Boundary communication-in-one concepts
======================================

On CPU, the naive implementation of ghost halos for meshblocks is to
loop over each geometric sub-area of a ghost halo relevant for
communicating to neighboring meshblocks. In practice, on accelerators,
this is not performant, as launching a GPU kernel per sub-halo costs
significant latency.

To resolve this issue, we coalesce GPU kernels into hierarchically
paraellel loops, where the outer loop is over sub-halos, and the inner
loop is over the cells within a sub-halo. The information required to,
e.g., send, receive, prolongate, restrict information in each sub-halo
is precomputed and cached.

Per sub-halo, there is a struct, ``BndInfo_t``, which contains the
relevant information needed on device to perform loop operations. The
caching is enabled by two structs, ``BvarsSubCache_t`` which contains
all the arrays on host and device needed mostly containing these
``BndInfo_t`` objects, and ``BvarsCache_t`` which is essentially an
array of all the kinds of caches needed for the various kinds of
boundary operations performed.

We note that this infrastructure is more general than just ghost halos.
The same machinery is used for communicating the interiors of meshblocks 
that are restricted and/or prolongated between geometric multi-grid levels. 
Additionally, with the ownership model for non-face fields, the basic 
communication infrastructure could deal with flux correction as well 
(although currently flux correction uses a somewhat separate code path).

Buffer subsets
--------------

Sometimes it is desirable, for example for custom prolongation and
restriction or to communicate on a single geometric multi-grid level, 
to loop over a subset of the ghost sub-halos, rather than
all halos at once. This is enabled by the ``buffer_subsets`` and
``buffer_subsets_h`` arrays, which are contained in ``BvarsSubCache_t``.
The ``buffer_subsets`` array is a matrix, where the rows index the
subset, and the columns point to the indices in the ``bnd_info`` array
containing the subset of sub-halos you wish to operate on.

To communicate across a particular boundary type, the templated 
boundary communication routines (see :ref:`boundary_comm_tasks`) 
should be instantiated with the desired ``BoundaryType``, i.e. 

.. code:: cpp 

  SendBoundBufs<BoundaryType::gmg_restrict_send>(md); 

The different ``BoundaryType``\ s are: 

- ``any``: Communications are performed between all leaf blocks (i.e. the
  standard Parthenon grid that does not include multi-grid related blocks). 
- ``local``: Communications are performed between all leaf blocks that 
  are on the current rank. *Currently, this option should not be used 
  because there are possibly associated bugs. This and nonlocal would 
  only be used as a potential performance enhancement, calling both 
  should result in the same communication as just calling 
  BoundaryType::any.*
- ``nonlocal``: Communications are performed between all leaf blocks that 
  are on different ranks than the current rank. *Currently, this option 
  should not be used because there are possibly associated bugs.*
- ``flxcor_send`` and ``flxcor_recv``: Used for flux correction 
  routines, see below.
- ``gmg_same``: Communicates ghost halos between blocks in the 
  same geometric multi-grid level. 
- ``gmg_restrict_send`` and ``gmg_restrict_recv``: For restricting 
  block interiors between geometric multi-grid levels, i.e. inter-grid 
  communication. *It is probably not necessary to have separate 
  communicators for sending and receiving, but for now this is the way 
  if was written*
- ``gmg_prolongate_send`` and ``gmg_prolongate_recv``: For prolongating 
  block interiors between geometric multi-grid levels, i.e. inter-grid 
  communication. *It is probably not necessary to have separate 
  communicators for sending and receiving, but for now this is the way 
  if was written* 

.. _sparse boundary comm:

Sparse boundary communication
=============================

Communication between meshblocks at all stages is required to
synchronize fields that include ghost zones. For each pair of meshblocks
**a** and **b** that share a boundary, we define two *communication
channel*\ s (**a**->\ **b** and **b**->\ **a**) for each field that
communicates ghost zone data. This communication channel can be shared
amongst all stages contained in the ``MeshBlock`` field
``DataCollection<MeshBlockData<Real>> meshblock_data``, since the
communication during different stages doesn’t overlap. A communication
channel needs to contain some state that indicates if information has
been sent from block **a** but not received on block **b**, or if the
information has been received. Additionally, the communication channel
should contain storage if data is being communicated, which may not
always be the case for sparse variables. When the sender is sparse and
unallocated, no storage communication buffer storage is required since
the ghost zones of the receiver should be filled with default values.
When the sender is allocated, we always allocate buffer storage.

For (potentially) sparse variables, there should be five possible states
of the buffer:

-  ``sending``: The sender has filled the buffer with data meets the
   threshold for sparse allocation.
-  ``sending_null``: The sender is either unallocated, so that it just
   contains default data, or the sender is allocated and but all of the
   values in the communicated region fall below the sparse threshold.
-  ``received``: The receiver has successfully queried that information
   has been sent and the buffer contains data that is above the sparse
   threshold.
-  ``received_null``: The receiver has successfully queried that
   information has been sent and the buffer contains data that is above
   the sparse threshold.
-  ``stale``: The receiver no longer needs the information currently
   stored by the communication channel.

Information about the indices of the data being copied from the sender
and where they are copied to in the receiver is kept separate from the
communication channel in the current implementation. The only
information the communication channel has is the size of buffer
required.

Communication channels on the Mesh
----------------------------------

Communication channels on a mesh can be uniquely identified by the
global id (gid) of the sending block, the global id (gid) of the
receiving block, the variable for which data is being sent, and the
geometric element of the sending block that is shared with the receiving
block, with the geometric element being the volume, a face, an edge, or
a node of a block. The geometric element is necessary for unique
identification, since with periodic boundary conditions two blocks may
share more than a single geometric element. Each block has 27 geometric
elements which can each be represented with a vector extending from the
center of the block to the center of the geometric element (e.g. the
volume of the block corresponds to (0, 0, 0),the top face of the block
would be represented by (0, 0, 1), the bottom face by (0,0,-1), some
edge by (0, 1, -1), and a vertex to (1, 1, 1)). This vector can be
flattened as
``geom_idx = (vec.x + 1) + 3 * (vec.y + 1) * 9 * (vec.z + 1)`` to assign
a unique element to each index. Also note that if two blocks share an
element (or at least part of an element in the case of a multi-level
mesh), the corresponding geometric element index of the second element
is obviously
``geom_idx = (vec.x - 1) + 3 * (vec.y - 1) * 9 * (vec.z - 1)`` when
``vec`` is the offset vector of the first element. This information is
stored in a ``NeighborIndexes`` object.

In practice, we denote each channel by a unique key
``std::tuple<int, int, std::string, int>{send_gid, receive_gid, var_name, geometric_element_index_of_sender}``,
so that the ``Mesh`` can contain a map from these keys to communication
channels. Then, at each remesh, sending blocks and blocks that are
receiving from blocks on a different rank can create new communication
channels and register them in this map. 

MPI Communication IDs
~~~~~~~~~~~~~~~~~~~~~

For MPI commnunication, we need to uniquely identify MPI messages
between an ordered pair of ranks. In an easy world, we would do all
communication on ``MPI_COMM_WORLD`` and just create a unique tag from
the elements of the unique key tuple by doing something like
``tag = num_blocks_total * nvars * 27 * send_gid + nvars * 27 * receive_gid + 27 * var_idx + geometric_element_index_of_sender``,
but this creates sparsely populated tag numbers and quickly blows
through the maximum tag limit of 32767 defined by the MPI standard. In
practice, we create an MPI communicator associated with each variable
(see ``Mesh::SetupMPIComms()`` and ``Mesh::GetMPIComm(std::string)``)
and for each pair of ranks fill a
``std::map<UnorderedPair({send_gid, send_geom_elem}, {recv_gid, recv_geom_elem}), int>``
contained in the ``Mesh`` with zeros. Since ``std::map`` is always
sorted and we define comparators for ``UnorderedPair``, we end up with
the same ordering of the map on both ranks of the pair. We can then
iterate through the map and assign a unique, densely populated tag to
each pair in the map, which does not blow through the MPI tag limit. The
same tags can obviously be re-used in each communicator.

Utilities classes for boundary communication
--------------------------------------------

``class CommBuffer<T>``
~~~~~~~~~~~~~~~~~~~~~~~

This general idea is implemented in the ``CommBuffer<T>`` class
template, which contains a field ``T buf_`` and access to that field
through the method ``T& buffer()``. ``T`` is assumed to be some type
that can act as a buffer for storing the communicated data that has
methods ``size()`` and ``data()``, the latter of which must give access
to the underlying storage that can be used by the MPI communication
calls. ``CommBuffer`` has the public methods:

-  ``T& buffer()``: Gives access to the the actual data buffer for
   filling and reading.
-  ``void Allocate()``: Which allocates storage for the buffer and can
   be called internally by ``TryReceive()`` when non-null data is being
   communicated by a block on a different rank.
-  ``void Send()``: If the underlying storage buffer is allocated, sets
   the state of the buffer to ``sending``. If the underlying storage
   buffer is unallocated, set the state of the buffer to
   ``sending_null``. Also starts asynchronous MPI send if sender and
   receiver are on separate ranks.
-  ``void SendNull()``: Sets the buffer state to ``sending_null``. Also
   starts asynchronous MPI send of a zero length buffer if sender and
   receiver are on separate ranks.
-  ``void TryStartReceive()``: If on same rank, this does nothing. If on
   different ranks and irecv_started flag is set, does nothing. If on
   different ranks and it is a receiving buffer and
   ``do_sparse_allocation = false`` for the buffer, posts an
   ``MPI_Irecv`` right away, allocates the buffer if it is not already
   allocated, and flags that ``MPI_Irecv`` has been called. If on
   different ranks and it is a receiving buffer and
   ``do_sparse_allocation = true``, calls ``MPI_Iprobe`` to see if a
   message is available. If there is a message, check it is sending data
   or sending null, allocates or deallocates the buffer as necessary,
   and then posts an ``MPI_Irecv`` and sets the ``MPI_Irecv`` flag.
-  ``bool TryReceive()``: If on same rank, checks if state is
   ``sending`` or ``sending_null`` and sets to ``received`` or
   ``received_null``, respectively, and returns ``true``. If on
   different ranks, first calls ``TryStartReceive()`` then, if the
   ``MPI_Irecv`` has been posted tests wether or not it has been
   completed. If it has, sets the buffer state to ``received`` or
   ``received_null`` depending on the size of the incoming message and
   returns ``true``. Otherwise returns ``false``.
-  ``Stale()``: Sets the state to ``stale``.

as well as copy constructors, assignment operators, etc. The constructor
of ``CommBuffer`` is called as

.. code:: cpp

   CommBuffer<T>(mpi_message_tag, sender_rank, receiver_rank, mpi_communicator,
               [...capture necessary stuff...](){ 
                 return ...allocated object of type T that has the desired size...; 
               }, do_sparse_allocation);

The lambda passed to the constructor is stored as a field in the class
and is called when the internal storage buffer needs to be allocated
(see ``BuildBoundaryBuffers`` in ``sparse_bvals_cc_in_one.cpp`` for an
example usage). Aside from during construction, there should be no
difference in useage between a same rank to same rank ``CommBuffer`` and
a separate rank ``CommBuffer``.

*Note that setting ``do_sparse_allocation = true`` minimizes the memory
allocated for sparse variables but may result in slower MPI
communication since ``MPI_Irecv`` can’t be posted until the incoming
message size is known. In simple tests, it appears that this does not
give a significant slow down, so all ``Metadata::Sparse`` variables use
sparse allocation. If in the future there is a need to turn this on and
off on a per variable there is a flag,
``Metadata::SparseCommunication``, that can be set for variables to make
them use this memory minimizing communication pattern. This would also
be required a change in ``BuildBoundaryBuffers`` switching how the flag
for using sparse buffers is set.*

``class ObjectPool<T>``
~~~~~~~~~~~~~~~~~~~~~~~

An ``ObjectPool`` hands out reference counted objects that publicly
inherit from ``T``, which is assumed to be something like a Kokkos view
which has an assignment operator and copy ctor that perform shallow
copies. Rather than creating a new object each time one is requested, an
``ObjectPool`` recycles previously created objects that have been
released back to the pool to limit the number of times that objects need
to allocated (in particular on device). When the class method ``Get()``
is called a pre-allocated, free resource is handed out if one is
available, otherwise a new resource is created.

An object pool has two types of objects it can hand out,

.. code:: cpp

   class ObjectPool<T>::weak_t : public T {...};
   class ObjectPool<T>::owner_t : public weak_t {...};

both of which contain a pointer to the object pool that handed them out
and a key that identifies the unique resource in the pool that they
reference. An ``owner_t`` object contributes to the reference count for
a particular resource. When the reference count goes to zero in an
``owner_t`` dtor, the underlying resource is added to the stack of
available objects and the key associated with the resource is
invalidated, but a copy of it still exists (so, for instance, the
reference count of an underlying ``Kokkos::View`` will not go to zero
and its memory will not be released). A ``weak_t`` object does not
contribute to the reference count and its underlying resource can become
invalid. The validity of an object can be checked with the member method
``bool IsValid()``.

The mechanism by which new pool objects are created is specified by a
lambda passed to the ``ObjectPool`` constructor:

.. code:: cpp

   template <class T>
   using dev_arr_t = typename Kokkos::View<T *, Kokkos::LayoutRight, Kokkos::CudaSpace>;
   int N = 100;
   Pool<dev_arr_t<double>>([N](){ return dev_arr_t<double>("test pool", N); });

On host a lot of this functionality could be replicated with
``shared_ptr`` I think, but it is somewhat useful for these objects to
be able to exist on device (even though the reference counting doesn’t
work there).

Sparse boundary communication implementation
--------------------------------------------

The tasks for sparse cell centered variable boundary communication
pretty closely mirror the old ``bvals_in_one`` tasks but allow for
allocation and deallocation of the communication buffers on the fly. The
``BndInfo`` class (which comes from the old bvals_in_one implementation)
stores the index ranges and data arrays necessary for copying from the
mesh to a buffer or vice versa. These are cached within a one
dimensional par array so that loading and unloading of buffers can done
“in one” kernel launch on device (in exactly the same way as the old
``bvals_in_one`` setup). The ``BndInfo`` objects are cached in each
``MeshData<Real>`` object within a ``BvarsCache_t`` object (which can
contain a number of sub-caches ``BvarsSubCache_t`` that correspond to
different subsets of communication channels) to limit the number of deep
copies from host to device. We also add a map of object pools containing
pools for various buffer sizes.

.. code:: cpp

   template <typename T>
   using buf_pool_t = ObjectPool<BufArray1D<T>>
   std::unordered_map<int, buf_pool_t<Real>> pool_map;

As well as the map from communication channel keys to communication
buffers associated with each channel

.. code:: cpp

   using channel_key_t = std::tuple<int, int, std::string, int>;
   std::unordered_map<channel_key_t, CommBuffer<buf_pool_t<Real>::owner_t>> boundary_comm_map;

Note that every stage shares the same ``CommBuffer``\ s, but we keep
separate buffers for boundary value communication and flux correction
communication so these operations can occur concurrently if necessary.

Send and Receive Ordering
~~~~~~~~~~~~~~~~~~~~~~~~~

In each cache, we build a
``std::vector<CommBuffer<....>*> send_buf_vec, recv_buf_vec`` which
contains pointers to every communication channel associated with
sending/receiving from a ``MeshData`` object. This is useful for a
couple of reasons. First, this speeds up the code by reducing the number
of times a ``std::unordered_map`` lookup from the ``boundary_comm_map``
is required. Second, we allow for any ordering of ``*_buf_vec`` (by
including a secondary array for indexing between sequential index
defined by the order of ``ForEachBoundary`` to the index in the buffer
cache). The ordering of this vector determines the order in which
``MPI_Isend`` and ``MPI_Irecv`` calls are posted, which can impact the
communication performance. *This is something that can be experimented
with for optimal performance. Strangely, I have seen the best results on
test problems for random ordering, but it is not clear if this
generalizes to more realistic problems not being run with all ranks on
the same node. See ``InitializeBufferCache(...)`` for how to choose the
ordering.*

.. _boundary_comm_tasks:

Boundary Communication Tasks
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. topic:: ``BuildBoundaryBuffers(std::shared_ptr<MeshData<Real>>&)``

  * Iterates over communication channels sending or receiving from blocks in
    ``md``. For every sending channel it creates a communication channel for
    each in the ``Mesh::boundary_comm_map``. For receiving channels where
    the blocks are on different ranks, it also creates a receiving channel
    in ``Mesh::boundary_comm_map`` since the sender will not add this
    channel on the current rank. Also creates new ``buf_pool_t``\ s for the
    required buffer sizes if they don’t already exist. Note that no memory
    is saved for the communication buffers at this point.
  * This is called
    during ``Mesh::Initialize(...)`` and during
    ``EvolutionDriver::InitializeBlockTimeStepsAndBoundaries()`` and before
    this task is called ``Mesh::boundary_comm_map`` is cleared.
    **This should not be called in downstream code.**

.. topic:: ``SendBoundBufs<bound_type>(std::shared_ptr<MeshData<Real>>&)``

  * Iterates over boundaries of ``bound_type``, which supports ``any``,
    ``local`` which implies the communicating blocks are on the same rank,
    and ``nonlocal`` which implies differing ranks for the two blocks.
    Could have one task with ``any`` or split communication into separate
    ``local`` and ``nonlocal`` tasks.
  * ``SendBoundaryBuffers`` is just an
    alias for ``SendBoundBufs<any>`` to ensure backward compatibility.
  * Allocates buffers if necessary based on allocation status of block
    fields and checks if ``MeshData::send_bnd_info`` objects are stale.
  * Rebuilds the ``MeshData::send_bnd_info`` objects if they are stale
  * Restricts where necessary
  * Launches kernels to load data from fields
    into buffers, checks whether any of the data is above the sparse
    allocation threshold.
  * Calls ``Send()`` or ``SendNull()`` from all of
    the boundary buffers depending on their status.

.. topic:: ``StartReceiveBoundBufs<bound_type>(std::shared_ptr<MeshData<Real>>&)``

  * Iterates over boundaries of ``bound_type``, which supports ``any``,
    ``local`` which implies the communicating blocks are on the same rank,
    and ``nonlocal`` which implies differing ranks for the two blocks. This
    is a no-op for ``local`` boundaries.
  * Posts/tries to post an
    ``MPI_Irecv`` for receiving buffers. For performance, it is often
    necessary to call this early in the task list before the rest of the
    communication routines get called. Codes will produce correct results
    without ever calling this task though.

.. topic:: ``ReceiveBoundBufs<bound_type>(std::shared_ptr<MeshData<Real>> &)``

  * Iterates over boundaries of ``bound_type``, which supports ``any``,
    ``local`` which implies the communicating blocks are on the same rank,
    and ``nonlocal`` which implies differing ranks for the two blocks.
    Could have one task with ``any`` or split communication into separate
    ``local`` and ``nonlocal`` tasks.
  * ``ReceiveBoundaryBuffers`` is just an alias for ``ReceiveBoundBufs<any>``
    to ensure backward compatibility.
  * Tries to receive from each of the receive channels associated with
    ``md`` of the chosen boundary type.
  * If the receive is succesful, and
    allocation of the associated field is required, allocate it.

.. topic:: ``SetBounds<bound_type>(std::shared_ptr<MeshData<Real>>& md)``

  * Iterates over boundaries of ``bound_type``, which supports ``any``,
    ``local`` which implies the communicating blocks are on the same rank,
    and ``nonlocal`` which implies differing ranks for the two blocks. Could
    have one task with ``any`` or split communication into separate
    ``local`` and ``nonlocal`` tasks.
  * ``SetBoundaries`` is just an alias for ``SetBounds<any>`` to ensure backward compatibility.
  * Check if ``MeshData::recv_bnd_info`` needs to be rebuilt because of changed
    allocation status.
  * Rebuild ``MeshData::recv_bnd_info`` if necessary.
  * Launch kernels to copy from buffers into fields or copy default data
    into fields if sending null.
  * Stale the communication buffers.
  * Restrict ghost regions where necessary to fill prolongation stencils.

Flux Correction Tasks
~~~~~~~~~~~~~~~~~~~~~

Flux correction is required to ensure conservation at fine-coarse boundaries, as the 
sum of fluxes on a fine block corresponding to a single flux on a coarse neighbor block 
is not guaranteed to be equal to the coarse flux. To ensure conservation, coarse fluxes
at fine-coarse boundaries are replaced with the sums of fine fluxes in an interblock 
communication step separate from filling ghost zones. Nevertheless, Parthenon uses the 
same machinery for flux correction as ghosts communication since they are essentially the 
same operation, just on different fields and different index ranges. 

Flux correction can be implemented in a task list via the calls 
- ``StartReceiveBoundBufs<BoundaryType::flxcor_recv>(std::shared_ptr<MeshData<Real>>&)``
- ``SendBoundBufs<BoundaryType::flxcor_send>(std::shared_ptr<MeshData<Real>>&)``
- ``ReceiveBoundBufs<BoundaryType::flxcor_recv>(std::shared_ptr<MeshData<Real>>&)``
- ``SetBoundBufs<BoundaryType::flxcor_recv>(std::shared_ptr<MeshData<Real>>&)``
which cause fields with `Metadata::Flux` set to restrict and then communicate only shared 
elements only on fine to coarse boundaries (as was done in legacy Parthenon versions of 
flux correction that only worked for cell variable flux correction). Notice that the send 
operation requires a different ``BoundaryType`` than the receive operation, unlike regular
boundary communication. All of these tasks can be added to a task list in one call using 
``AddFluxCorrectionTasks``.

Flux correction for sparse variables and dense variables is very
similar, the only difference being that for sparse variables if either
the fine or the coarse block is unallocated no flux correction occurs.
Flux correction communication cannot trigger allocation. 

For backwards compatibility, we keep the aliases 

- ``StartReceiveFluxCorrections`` = ``StartReceiveBoundBufs<BoundaryType::flxcor_recv>``
- ``LoadAndSendFluxCorrections`` = ``SendBoundBufs<BoundaryType::flxcor_send>``
- ``ReceiveFluxCorrections`` = ``ReceiveBoundBufs<BoundaryType::flxcor_recv>`` 
- ``SetFluxCorrections`` = ``SetBoundBufs<BoundaryType::flxcor_recv>``

