CMAKE=cmake
if command -v cmake3 > /dev/null
then
    CMAKE=cmake3
fi

NUM_PROCESSES=${1:-}

THIRDPARTY_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
