# Theater Booking Service

This project is a UDP-based booking service for movies, theaters, and seat reservations. The application listens on a UDP socket, decodes text commands, stores session state per client, and schedules operations on a thread pool for execution.

The system is designed to support multiple clients concurrently, while keeping a per-session state and using a single in-memory database instance for movie and seat data.

## How the software works

The runtime is organized in a few main layers:

- Application: owns the UDP socket and the main loop
- UserSessionManager: stores user state by session id
- DEXDecode: parses textual commands into executable operations
- ThreadPool: executes queued tasks asynchronously
- MovieSessionDatabase: keeps the in-memory movie and seat data

The request flow is:

1. The server receives a UDP datagram.
2. The payload is decoded as a command such as SELECT_MOVIE or BOOK.
3. The command is translated into a callable operation.
4. The operation is added to the thread pool queue.
5. The thread pool executes it later, updating the session and/or database.
6. The application sends the response back to the sender UDP port.

This makes the server suitable for concurrent workloads without forcing each client to block in the receive loop.

## Supported commands

The application accepts commands in this format:

- session_id GET_MOVIES
- session_id SELECT_MOVIE MovieName
- session_id SELECT_THEATER TheaterName
- session_id BOOK Seat1 Seat2

Examples:

- client-1 GET_MOVIES
- client-1 SELECT_MOVIE Inception
- client-1 SELECT_THEATER Cinema 1
- client-1 BOOK A1 A2

The application responds with strings such as:

- MOVIES:Inception|TheMatrix
- THEATERS:IMAX|Cinema 2
- SEATS:A1|A2|A3
- BOOKED
- ERROR:INVALID_COMMAND
- ERROR:GET_MOVIES
- ERROR:SEAT_UNAVAILABLE

## Project structure

- code/include: public headers
- code/src: implementation files
- tests: automated tests for database, protocol decoding, and application flows

The main executable is built into:

- build/bin/BookingServiceExecutable

## Prerequisites

On Linux and macOS, you need:

- CMake 3.20 or newer
- a modern C++ compiler such as GCC, Clang, or Apple Clang
- standard Unix socket support

## Build

From the project root:

```bash
cmake -S . -B build
cmake --build build --parallel
```

You can also build in Release mode if desired:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
```

## Run the server

Start the application:

```bash
./build/bin/BookingServiceExecutable
```

The default bind address is 127.0.0.1 and the default UDP port is 9000.

If you want to keep it running in the background, use:

```bash
./build/bin/BookingServiceExecutable > server.log 2>&1 &
```

## Sending commands from the terminal

Because this is a UDP socket service, the client side usually sends a datagram and reads the response from the socket that it opened for receiving.

### Using printf

You can send a single command directly with printf:

```bash
printf '%s' 'client-1 SELECT_MOVIE Inception' | nc -u -w 1 127.0.0.1 9000
```

This sends the packet, but the server response will only be visible if the sender is also listening on a UDP port to receive it.

### Using nc and a listener port

This is the most practical pattern for testing in Linux and macOS.

Terminal 1: start a UDP listener on a local port:

```bash
nc -u -l -k 127.0.0.1 9999
```

Terminal 2: send a command from source port 9999 so the server replies back to that port:

```bash
printf '%s' 'client-1 SELECT_MOVIE Inception' | nc -u -w 1 -p 9999 127.0.0.1 9000
```

The server will answer to the sender port, and the listening socket on 9999 will receive the response.

### Using nc for booking

```bash
printf '%s' 'client-1 SELECT_THEATER Cinema 1' | nc -u -w 1 -p 9999 127.0.0.1 9000
printf '%s' 'client-1 BOOK A1' | nc -u -w 1 -p 9999 127.0.0.1 9000
```

### Using bash /dev/udp

On Linux, bash can also send UDP traffic without nc:

```bash
printf '%s' 'client-1 GET_MOVIES' >/dev/udp/127.0.0.1/9000
```

This is handy for quick smoke tests, but it is less convenient for receiving the reply.

## Example client conversation

A typical interaction looks like this:

1. Send a movie selection:

```bash
printf '%s' 'client-1 SELECT_MOVIE Inception' | nc -u -w 1 -p 9999 127.0.0.1 9000
```

Expected response:

```text
THEATERS:IMAX
```

2. Select a theater:

```bash
printf '%s' 'client-1 SELECT_THEATER IMAX' | nc -u -w 1 -p 9999 127.0.0.1 9000
```

Expected response:

```text
SEATS:A1|A2|A3|...
```

3. Book a seat:

```bash
printf '%s' 'client-1 BOOK A1' | nc -u -w 1 -p 9999 127.0.0.1 9000
```

Expected response:

```text
BOOKED
```

## Docker

This project also supports building and running inside Docker.

### Build the image

From the project root:

```bash
docker build -t theater-booking-service .
```

This Dockerfile uses a multi-stage build:

- builder: installs CMake and Ninja and compiles the project in Release mode
- tester: runs the configured CTest suite inside the image
- runtime: produces a smaller final image that contains only the executable

### Run the container

Run the server in a container:

```bash
docker run --rm -p 9000:9000/udp --name theater-booking-service theater-booking-service
```

This exposes UDP port 9000 from the container to the host. The service listens on 127.0.0.1 inside the container and accepts commands over UDP.

### Run the container in detached mode

```bash
docker run -d --rm -p 9000:9000/udp --name theater-booking-service theater-booking-service
```

### Rebuild after source changes

```bash
docker build --no-cache -t theater-booking-service .
```

### Container notes

- The service is meant to run as a UDP server and is not a web application.
- The image compiles the project inside Docker, so the build process is fully reproducible in CI or local containers.
- The runtime container uses the compiled binary directly, without requiring local toolchain installation.

## GitHub Actions

This repository includes a GitHub Actions workflow for continuous integration.

The CI pipeline is intended to validate the project automatically on every push and pull request.

Typical flow:

1. Checkout the repository
2. Configure the build with CMake
3. Compile the project
4. Execute the CTest suite
5. Fail the workflow if any tests or builds break

The workflow is usually stored in:

- .github/workflows/

Example command used by CI:

```bash
cmake -S . -B build
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

This ensures the project compiles in a clean environment and that the database, protocol decoder and application tests remain green over time.

## Testing

The project includes CTest-based tests. To run all tests:

```bash
ctest --test-dir build --output-on-failure
```

Or build and run everything together:

```bash
cmake -S . -B build
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

## Troubleshooting

- If the application does not start, confirm the port 9000 is not already in use.
- If the client does not receive a response, make sure the sender socket is bound to a local port and is listening for UDP responses.
- If commands are rejected, verify the session id is present and the selected movie or theater was chosen first.
- If the server fails to book a seat, it usually means the seat is already reserved or the session has not selected the expected movie/theater.

## Notes

- This service is intentionally in-memory and does not persist data to disk.
- The database is static inside the application lifetime.
- The thread pool is used to decouple command processing from the socket receive loop.
- The application does not include a CLI command parser or a command-line menu; it is socket-only.
