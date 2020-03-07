#! /bin/bash
docker run -ti -v $(pwd):/workspace -w /workspace -u root reaverproject/llvm-boost:70-1.69.0