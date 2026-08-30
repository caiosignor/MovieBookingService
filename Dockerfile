# ─────────────────────────────────────────────────────────────────────────────
# Stage 1 — builder
# Compila o projeto com GCC 14 + CMake + Ninja em Release
# ─────────────────────────────────────────────────────────────────────────────
FROM gcc:14-bookworm AS builder

RUN apt-get update && apt-get install -y --no-install-recommends \
        cmake \
        ninja-build \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /src

COPY CMakeLists.txt .
COPY code/CMakeLists.txt code/
COPY tests/CMakeLists.txt tests/
COPY code/ code/
COPY tests/ tests/

RUN cmake -S . -B build \
        -G Ninja \
        -DCMAKE_BUILD_TYPE=Release \
    && cmake --build build --parallel "$(nproc)"

# ─────────────────────────────────────────────────────────────────────────────
# Stage 2 — tester
# Roda a suíte de testes registrada no CTest
# ─────────────────────────────────────────────────────────────────────────────
FROM builder AS tester

WORKDIR /src/build
RUN ctest --output-on-failure --test-dir /src/build

# ─────────────────────────────────────────────────────────────────────────────
# Stage 3 — runtime
# Imagem mínima: executável do build compilado
# ─────────────────────────────────────────────────────────────────────────────
FROM debian:bookworm-slim AS runtime

RUN apt-get update && apt-get install -y --no-install-recommends libstdc++6 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY --from=builder /src/build/bin/BookingServiceExecutable ./BookingServiceExecutable

ENV APP_HOST=0.0.0.0
ENV APP_PORT=9000

EXPOSE 9000/udp

ENTRYPOINT ["./BookingServiceExecutable"]
CMD ["--host", "0.0.0.0", "--port", "9000"]
