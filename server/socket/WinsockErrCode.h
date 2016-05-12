socket()
Value	Error code				Meaning 
10093	WSANOTINITIALISED		A successful WSAStartup call must occur before using this function. 
10050	WSAENETDOWN				The network subsystem or the associated service provider has failed. 
10047	WSAEAFNOSUPPORT			The specified address family is not supported. 
10036	WSAEINPROGRESS			A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function. 
10024	WSAEMFILE				No more socket descriptors are available. 
10055	WSAENOBUFS				No buffer space is available. The socket cannot be created. 
10043	WSAEPROTONOSUPPORT		The specified protocol is not supported. 
10041	WSAEPROTOTYPE			The specified protocol is the wrong type for this socket. 
10044	WSAESOCKTNOSUPPORT		The specified socket type is not supported in this address family. 


select()
Value	Error code				Meaning 
10093	WSANOTINITIALISED		A successful WSAStartup call must occur before using this function. 
10014	WSAEFAULT				The Windows Sockets implementation was unable to allocate needed resources for its internal operations, or the readfds, writefds, exceptfds, or timeval parameters are not part of the user address space. 
10050	WSAENETDOWN				The network subsystem has failed. 
10022	WSAEINVAL				The time-out value is not valid, or all three descriptor parameters were null. 
10004	WSAEINTR				A blocking Windows Socket 1.1 call was canceled through WSACancelBlockingCall. 
10036	WSAEINPROGRESS			A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function. 
10038	WSAENOTSOCK				One of the descriptor sets contains an entry that is not a socket. 


accept()
Value	Error code				Meaning 
10093	WSANOTINITIALISED		A successful WSAStartup call must occur before using this function. 
10050	WSAENETDOWN				The network subsystem has failed. 
10014	WSAEFAULT				The addrlen parameter is too small or addr is not a valid part of the user address space. 
10004	WSAEINTR				A blocking Windows Sockets 1.1 call was canceled through WSACancelBlockingCall. 
10036	WSAEINPROGRESS			A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function. 
10022	WSAEINVAL				The listen function was not invoked prior to accept. 
10024	WSAEMFILE				The queue is nonempty upon entry to accept and there are no descriptors available. 
10055	WSAENOBUFS				No buffer space is available. 
10038	WSAENOTSOCK				The descriptor is not a socket. 
10045	WSAEOPNOTSUPP			The referenced socket is not a type that supports connection-oriented service. 
10035	WSAEWOULDBLOCK			The socket is marked as nonblocking and no connections are present to be accepted. 


//file end

