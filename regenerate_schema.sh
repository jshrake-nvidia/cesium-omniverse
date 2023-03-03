#/bin/bash

PROJECT_ROOT=`dirname -- "$( readlink -f -- "$0"; )"`
SCHEMA_INPUT_PATH="$PROJECT_ROOT/exts/cesium.omniverse/schemas/cesium_schemas.usda"
SCHEMA_OUTPUT_PATH="$PROJECT_ROOT/src/plugins/CesiumUsdSchemas/src/CesiumUsdSchemas"
NVIDIA_USD_BINS="$PROJECT_ROOT/extern/nvidia/_build/target-deps/usd/release/bin"
NVIDIA_PYTHON_BINS="$PROJECT_ROOT/extern/nvidia/_build/target-deps/python/bin"
NVIDIA_PYTHON_EXECUTABLE="$NVIDIA_PYTHON_BINS/python3"
NVIDIA_PYTHON_LIBS="$PROJECT_ROOT/extern/nvidia/_build/target-deps/python/lib"
NVIDIA_USD_PYTHON_LIBS="$PROJECT_ROOT/extern/nvidia/_build/target-deps/usd/release/lib/python"

export PYTHONPATH="$NVIDIA_USD_PYTHON_LIBS:$PYTHONPATH"
export PATH="$NVIDIA_USD_BINS:$NVIDIA_PYTHON_LIBS:$NVIDIA_PYTHON_BINS:$NVIDIA_USD_PYTHON_LIBS:$PATH"

$NVIDIA_PYTHON_EXECUTABLE -m pip install jinja2

# Clear out the old files.
rm $SCHEMA_OUTPUT_PATH/*

# Generate the new files.
usdGenSchema $SCHEMA_INPUT_PATH $SCHEMA_OUTPUT_PATH

# Move the generatedSchema.usda and plugInfo.json files up.
mv "$SCHEMA_OUTPUT_PATH/generatedSchema.usda" "$SCHEMA_OUTPUT_PATH/../../generatedSchema.usda.in"
mv "$SCHEMA_OUTPUT_PATH/plugInfo.json" "$SCHEMA_OUTPUT_PATH/../../plugInfo.json.in"

# Delete the Pixar junk from the first 23 lines of the generated code.
for f in $SCHEMA_OUTPUT_PATH/*; do
  sed -i '1,23d' "$f"
done