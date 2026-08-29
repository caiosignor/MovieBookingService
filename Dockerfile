# ─────────────────────────────────────────────────────────────────────────────
# Stage 1 — builder
# Compila o projeto com GCC 14 + CMake + Ninja em Release
# ─────────────────────────────────────────────────────────────────────────────
FROM gcc:14-bookworm AS builder

# Instala CMake e Ninja (versões recentes via apt)
RUN apt-get update && apt-get install -y --no-install-recommends \
        cmake \
        ninja-build \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /src

# Copia apenas os manifestos de build primeiro (melhor cache)
COPY CMakeLists.txt .
COPY src/CMakeLists.txt src/
COPY tests/CMakeLists.txt tests/

# Copia o código-fonte e headers
COPY src/    src/
COPY include/ include/
COPY tests/  tests/

# Configura e compila em Release
RUN cmake -S . -B build \
        -G Ninja \
        -DCMAKE_BUILD_TYPE=Release \
    && cmake --build build --parallel "$(nproc)"

# ─────────────────────────────────────────────────────────────────────────────
# Stage 2 — tester
# Roda a suíte de testes registrada no CTest. Uma falha interrompe o build de CI.
# ─────────────────────────────────────────────────────────────────────────────
FROM builder AS tester

WORKDIR /src/build

RUN ctest --output-on-failure --test-dir /src/build

# ─────────────────────────────────────────────────────────────────────────────
# Stage 3 — runtime
# Imagem mínima: apenas o binário copiado do builder.
# ─────────────────────────────────────────────────────────────────────────────
FROM debian:bookworm-slim AS runtime

# Usuário não-root para segurança
RUN useradd --no-create-home --shell /bin/false appuser
USER appuser

WORKDIR /app

# Copia apenas o executável compilado do stage builder
COPY --from=builder /src/build/bin/TheaterBookingSoftware .

ENTRYPOINT ["./TheaterBookingSoftware"]
