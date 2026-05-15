# file_copy_project

## Local copy

```sh
make
./main <src_dir> <dst_dir>
```

## TCP file transfer

Terminal 1: receive a file into `recv_dir`.

```sh
cd tcp_socket
make
./tcp_file_server 8080 recv_dir
```

Terminal 2: send a file to the server.

```sh
./tcp_file_client 127.0.0.1 8080 path/to/file.txt
```

Protocol:

1. filename length: 2 bytes, network byte order
2. file size: 8 bytes, big endian
3. filename bytes
4. file contents
