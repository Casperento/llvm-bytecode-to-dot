#ifndef LLVM_BYTECODE_TO_DOT_BYTECODETODOT_H
#define LLVM_BYTECODE_TO_DOT_BYTECODETODOT_H

#include <bits/stdc++.h>
#include <llvm/Pass.h>
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/Transforms/Utils/LCSSA.h"
#include "llvm/Transforms/Utils/Mem2Reg.h"

namespace llvm {
    class BytecodeToDotPass : public PassInfoMixin<BytecodeToDotPass> {
    public:
        PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
        static bool isRequired() { return true; }
    };
}

#endif //LLVM_BYTECODE_TO_DOT_BYTECODETODOT_H
