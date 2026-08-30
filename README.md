# Theater Booking Service

A small UDP based backend service for booking movie theater seats.

The service receives commands through UDP, keeps the current session state for each client and processes requests using a thread pool.

The movie and theater data is kept in memory.

## Table of Contents

- [Summary](#summary)
- [How it works](#how-it-works)
- [Commands](#commands)
- [Project structure](#project-structure)
- [Requirements](#requirements)
- [Build](#build)
- [Running](#running)
- [Sending commands](#sending-commands)
- [Example](#example)
- [Docker](#docker)
- [GitHub Actions](#github-actions)
- [Tests](#tests)
- [Concurrency test](#concurrency-test)
- [Troubleshooting](#troubleshooting)
- [Notes](#notes)

## Summary

The project is a UDP based booking service for movies, theaters and seats.

The server receives commands, keeps the state of each client session and uses a thread pool to process requests.

The data is not persisted. Everything is kept in memory while the application is running.

## How it works

The main parts of the application are:

- `Application` - owns the UDP socket and receives requests
- `UserSessionManager` - keeps the state for each session
- `DEXDecode` - parses the received commands
- `ThreadPool` - executes the requests
- `MovieSessionDatabase` - stores movies, theaters and seats

The basic flow is:

1. Receive a UDP packet.
2. Decode the command.
3. Create the operation for that command.
4. Put it in the thread pool.
5. Execute the operation.
6. Send the response back to the client.

The receive loop doesn't have to wait for each request to finish.

## Commands

Commands have the following format:

```text
session_id GET_MOVIES
session_id SELECT_MOVIE MovieName
session_id SELECT_THEATER TheaterName
session_id BOOK Seat1 Seat2
```

For example:

```text
client-1 GET_MOVIES
client-1 SELECT_MOVIE Inception
client-1 SELECT_THEATER Cinema 1
client-1 BOOK A1 A2
```

Some possible responses:

```text
MOVIES:Inception|TheMatrix
THEATERS:IMAX|Cinema 2
SEATS:A1|A2|A3
BOOKED
ERROR:INVALID_COMMAND
ERROR:GET_MOVIES
ERROR:SEAT_UNAVAILABLE
```

## Project structure

```text
code/include    public headers
code/src        source files
tests           tests
```

The executable is generated at:

```text
build/bin/BookingServiceExecutable
```

## Requirements

You'll need:

- CMake 3.20+
- GCC, Clang or Apple Clang
- UDP socket support

No external libraries are required.

## Build

From the project directory:

```bash
cmake -S . -B build
cmake --build build --parallel
```

For a Release build:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
```

## Running

The default address and port are:

```text
127.0.0.1:9000
```

Run it with:

```bash
./build/bin/BookingServiceExecutable
```

Or specify them:

```bash
./build/bin/BookingServiceExecutable --host 127.0.0.1 --port 9000
```

For example:

```bash
./build/bin/BookingServiceExecutable --host 0.0.0.0 --port 9001
```

Short port option is also available:

```bash
./build/bin/BookingServiceExecutable -p 9000 --host 0.0.0.0
```

To run it in the background:

```bash
./build/bin/BookingServiceExecutable --host 0.0.0.0 --port 9000 > server.log 2>&1 &
```

## Sending commands

Since the server uses UDP, you can use `nc` for some quick tests.

For example:

```bash
printf '%s' 'client-1 SELECT_MOVIE Inception' | nc -u -w 1 127.0.0.1 9000
```

Depending on the `nc` version, receiving the response is easier if the client uses a fixed source port.

Start a listener:

```bash
nc -u -l -k 127.0.0.1 9999
```

Then send the command using port `9999`:

```bash
printf '%s' 'client-1 SELECT_MOVIE Inception' | nc -u -w 1 -p 9999 127.0.0.1 9000
```

The server sends the response to the source port of the request.

For a booking:

```bash
printf '%s' 'client-1 SELECT_THEATER Cinema 1' | nc -u -w 1 -p 9999 127.0.0.1 9000

printf '%s' 'client-1 BOOK A1' | nc -u -w 1 -p 9999 127.0.0.1 9000
```

On Linux, another quick option is `/dev/udp`:

```bash
printf '%s' 'client-1 GET_MOVIES' >/dev/udp/127.0.0.1/9000
```

This is mostly useful for sending packets. It's not very convenient for reading the response.

## Example

Select a movie:

```bash
printf '%s' 'client-1 SELECT_MOVIE Inception' | nc -u -w 1 -p 9999 127.0.0.1 9000
```

Response:

```text
THEATERS:IMAX
```

Then select the theater:

```bash
printf '%s' 'client-1 SELECT_THEATER IMAX' | nc -u -w 1 -p 9999 127.0.0.1 9000
```

Response:

```text
SEATS:A1|A2|A3|...
```

And book a seat:

```bash
printf '%s' 'client-1 BOOK A1' | nc -u -w 1 -p 9999 127.0.0.1 9000
```

Response:

```text
BOOKED
```

## Docker

The project can also be built and run using Docker.

Build the image:

```bash
docker build -t theater-booking-service .
```

The Dockerfile uses multiple stages. The project is built first, the tests are run and the final image only contains what is needed to run the service.

Run it:

```bash
docker run --rm \
  -p 9000:9000/udp \
  --name theater-booking-service \
  theater-booking-service
```

The container starts the server using:

```text
--host 0.0.0.0 --port 9000
```

Another port can be used too:

```bash
docker run --rm \
  -p 9001:9001/udp \
  --name theater-booking-service \
  theater-booking-service \
  --host 0.0.0.0 --port 9001
```

To run in background:

```bash
docker run -d --rm \
  -p 9000:9000/udp \
  --name theater-booking-service \
  theater-booking-service
```

The image also defines:

```text
APP_HOST=0.0.0.0
APP_PORT=9000
```

They can be changed when starting the container:

```bash
docker run --rm \
  -e APP_HOST=0.0.0.0 \
  -e APP_PORT=9000 \
  -p 9000:9000/udp \
  theater-booking-service
```

If you want to rebuild everything from scratch:

```bash
docker build --no-cache -t theater-booking-service .
```

## GitHub Actions

There is a GitHub Actions workflow that builds and tests the project on pushes and pull requests.

The basic CI flow is:

1. Checkout
2. Configure CMake
3. Build
4. Run tests

The commands are basically:

```bash
cmake -S . -B build
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

The workflow files are under:

```text
.github/workflows/
```

## Tests

The project uses CTest.

Run the tests with:

```bash
ctest --test-dir build --output-on-failure
```

Or:

```bash
cmake -S . -B build
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

There are tests for the database, command decoding and application behavior.

## Concurrency test

There is also an end-to-end test in:

```text
tests/testApplication.py
```

This one starts the actual server and creates 10 client threads. All clients try to book the same seat concurrently.

The point of the test is to check that only one request manages to book the seat.

It checks that:

- the server starts correctly
- the UDP socket is working
- multiple clients can send requests at the same time
- only one client can book the same seat

Run it with in root project dir:

```bash
cd tests
python3 testApplication.py
```

The executable needs to have already been built.

The test expects:

```text
../build/bin/BookingServiceExecutable
```

If needed, build it first:

```bash
cmake -S . -B build
cmake --build build --parallel
```

A successful test should result in something similar to:

```text
successful=1
failed=9
```

The exact output can vary, but the important part is that only one booking succeeds.

## Troubleshooting

If the server doesn't start, check if port `9000` is already being used.

If you don't get a response from the server, check that the client socket is using a source port where it can receive the UDP response.

If a command is rejected, check the session id and make sure the movie/theater was selected before trying to book.

If a booking fails with `SEAT_UNAVAILABLE`, the seat is probably already booked.

## Notes

This is an in-memory service, so nothing is persisted when the application stops.

The database lives for the lifetime of the application.

The thread pool is mainly there so the UDP receive loop doesn't have to execute every request itself.

There is no interactive CLI. The application is controlled through the UDP socket.
