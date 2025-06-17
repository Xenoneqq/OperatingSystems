# Preparation for the LAB 11

## Sockets - Sending and Retrieving Data

`Sockets` are used to send data between threads, processes and different divices via **internet or any other form of data transfer**.

Possible ways to send information are:

- `AF_UNIX / AF_LOCAL` - internaly in a given machine
- `AF_INET` - via IPv4
- `AF_INET6` - via IPv6

Ways of sending Packets are:

- `SOCK_STREAM` - **TCP** - Reliable system that ensures the data is recieved and sent in correct order. It's usually slower because of the checking procedure.

- `SOCK_DGRAM` - **UDP** - Not safe and not reliable system of sending data fast and in massive quantities. The order is not determined and the data can be lost without any mention.

## Using AF_INET

```C
#include <netinet/in.h>

struct sockaddr_in {
    sa_family_t    sin_family; /* address family: AF_INET */
    in_port_t      sin_port;   /* port in network byte order */
    struct in_addr sin_addr;   /* internet address */
};

/* Internet address. */
struct in_addr {
    uint32_t       s_addr;     /* address in network byte order */
};
```

`s_addr` and `sin_port` can be set with a variable returned from functions `htobe16(int host)` or `htons(int host)`. They require `include <arpa/inet.h>` to be used.

## Using AF_INET6

```C
#include <netinet/in.h>

struct sockaddr_in6 {
    sa_family_t     sin6_family;   /* AF_INET6 */
    in_port_t       sin6_port;     /* port number */
    uint32_t        sin6_flowinfo; /* IPv6 flow information */
    struct in6_addr sin6_addr;     /* IPv6 address */
    uint32_t        sin6_scope_id; /* Scope ID (new in 2.4) */
};

struct in6_addr {
    unsigned char   s6_addr[16];   /* IPv6 address */
};
```

`s6_addr` and `sin6_port` can be set with a variable returned from functions `htobe16(int host)` or `htons(int host)`. They require `include <arpa/inet.h>` to be used.

## Using AF_UNIX

```C
#define UNIX_PATH_MAX    108

struct sockaddr_un {
    sa_family_t sun_family;               /* AF_UNIX */
    char        sun_path[UNIX_PATH_MAX];  /* pathname */
}
```

Works like a File read and write system. Requires the socket *(File)* to be removed after use for example via `int unlink(const char *pathname)` from `unistd.h`

## Useful Functions

`int inet_pton(int af, const char *src, void *dst)`

Changes address string into an address structure.

- af - address family AF_INET , AF_INET6 etc.
- src - pointer to string address.
- dst - pointer to a buffer for the address structure.

`const char *inet_ntop(int af, const void *src, char *dst, socklen_t size)`

Changes an address structure into its text representation.

- af - address family AF_INET , AF_INET6 etc.
- src - pointer to structure address.
- dst - pointer to a buffer for the text representation.
- size - the amount of bytes in buffer

`struct hostent *gethostbyname(const char *name)`

Returns address or addresses related to the host name via the following structure:

```C
struct hostent {
    char  *h_name;            /* official name of host */
    char **h_aliases;         /* alias list */
    int    h_addrtype;        /* host address type */
    int    h_length;          /* length of address */
    char **h_addr_list;       /* list of addresses */
}
#define h_addr h_addr_list[0] /* for backward compatibility */
```

## Handling Sockets

To use any of these functions add these headers:

```C
#include <sys/socket.h>
#include <sys/types.h>
```

`int socket(int domain, int type, int protocol)`

Returns the descriptor of the created socket (-1 if failed)

- domain - One of the AF constants mentioned before `AF_INET, AF_INET6, AF_LOCAL, AF_UNIX`
- type - `SOCK_NONBLOCK` or `SOCK_CLOEXEC`
- protocol - 0 (automatic) or other specific one

`int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)`

Binds the socket to its name and address

- sockfd - the fd returned by the socket function
- sockaddr - buffor structure in memory to keep the sockaddress
- addrlen - size of the buffor structure

`int listen(int sockfd, int backlog)`

Associated with `SOCK_STREAM` **TCP** - listens for connections from clients

- sockfd - where to listen for connections (socked fd)
- backlog - (in short) max amount of clients (connections)

`int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)`

Used to connect to a given socket. In **TCP** it can be used once in **UDP** it can be used as many times as the user like.

- sockfd - where to connect (returned by socket)
- sockaddr - pointer to the created before structure
- addrlem - `sizeof(sockaddr)`