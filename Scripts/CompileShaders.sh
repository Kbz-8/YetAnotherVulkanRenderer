mkdir -p "$1/Shaders/Build/"

VERTEX="$1/Shaders/Vertex.nzsl"
FRAGMENT="$1/Shaders/Fragment.nzsl"
FRACTAL_COMPUTE="$1/Shaders/FractalCompute.nzsl"

VERTEX_SPV="$1/Shaders/Build/Vertex.spv"
FRAGMENT_SPV="$1/Shaders/Build/Fragment.spv"
FRACTAL_COMPUTE_SPV="$1/Shaders/Build/FractalCompute.spv"

if [ ! -f "$VERTEX_SPV" ]; then
	$1/Vendors/nzslc.x86_64 --compile=spv $VERTEX -o $1/Shaders/Build
	echo "Compiling vertex shader"
fi

if [ ! -f "$FRAGMENT_SPV" ]; then
	$1/Vendors/nzslc.x86_64 --compile=spv $FRAGMENT -o $1/Shaders/Build
	echo "Compiling fragment shader"
fi

if [ ! -f "$FRACTAL_COMPUTE_SPV" ]; then
	$1/Vendors/nzslc.x86_64 --compile=spv $FRACTAL_COMPUTE -o $1/Shaders/Build
	echo "Compiling fractal compute shader"
fi
