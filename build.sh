#!/bin/bash
set -ue -o pipefail

cmake="$(which cmake)" || { echo "CMake not found" 1>&2; exit 1; }
make="$(which make)" || { echo "Make not found" 1>&2; exit 1; }
markdown="$(which markdown)" || markdown=""

"${cmake}" -H. -Bbuild
"${make}" -C build
./build/bin/test_rebind

echo
echo "Build complete"
if [[ -n "${markdown}" ]]; then
    "${markdown}" README.md > build/readme.html
    echo "Documentation at build/readme.html"
else
    echo "'markdown' not found, unable to build documentation"
fi
