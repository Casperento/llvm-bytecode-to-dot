#include "../include/BytecodeToDot.h"

using namespace llvm;

PreservedAnalyses BytecodeToDotPass::run(Function &F,
                                         FunctionAnalysisManager &AM) {
    errs() << "\t\tFunction Name: " << F.getName() << "\n";
    for (BasicBlock &BB : F) {
        errs() << BB.getName() << "\n";
        for (Instruction &I : BB) {
            errs() << " ";
            if (I.getNameOrAsOperand() != "<badref>") {
                if (I.hasName())
                    errs() << "%";
                errs() << I.getNameOrAsOperand() << " = ";
            }
            errs() << I.getOpcodeName() << " ";

            size_t numOperands = I.getNumOperands();
            if (isa<CallInst>(I)) {
                std::string funcName = I.getOperand(--numOperands)->getNameOrAsOperand();
                errs() << "@" << funcName << "(";
                for (Use &U: I.operands()) {
                    Value *val = U.get();
                    if (val->getNameOrAsOperand() == funcName) {
                        continue;
                    }
                    if (val->hasName())
                        errs() << "%";
                    errs() << val->getNameOrAsOperand();
                    if (--numOperands != 0)
                        errs() << ", ";
                }
                errs() << ")";
            } else if (auto *PN = dyn_cast<PHINode>(&I)) {
                for (auto &bb : PN->blocks()) {
                    Value *val = PN->getIncomingValueForBlock(bb);
                    errs() << "[ ";
                    if (val->hasName())
                        errs() << "%";
                    errs() << val->getNameOrAsOperand() << ", ";
                    if (bb->hasName())
                        errs() << "%";
                    errs() << bb->getNameOrAsOperand() << " ]";
                    if (--numOperands != 0)
                        errs() << ", ";
                }
            } else {
                for (Use &U : I.operands()) {
                    Value *val = U.get();
                    if (val->hasName())
                        errs() << "%";
                    errs() << val->getNameOrAsOperand();
                    if (--numOperands != 0)
                        errs() << ", ";
                }
            }
            errs() << "\n";
        }
//        errs() << BB << "\n";
//        for (Instruction &I : instructions(F)) {
//            if (isa<BranchInst>(I))
//                errs() << "\t\t\tInstruction (type is BranchInst):\n";
//            else if (isa<ReturnInst>(I))
//                errs() << "\t\t\tInstruction (type is ReturnInst):\n";
//            else if (isa<CallInst>(I))
//                errs() << "\t\t\tInstruction (type is CallInst):\n";
//            else if (isa<BinaryOperator>(I))
//                errs() << "\t\t\tInstruction (type is BinaryOperator):" << "\n";
//            else if (isa<PHINode>(I))
//                errs() << "\t\t\tInstruction (type is PHINode):" << "\n";
//            else if (isa<StoreInst>(I))
//                errs() << "\t\t\tInstruction (type is StoreInst):" << "\n";
//            else if (isa<LoadInst>(I))
//                errs() << "\t\t\tInstruction (type is LoadInst):" << "\n";
//            else if (isa<TruncInst>(I))
//                errs() << "\t\t\tInstruction (type is TruncInst):" << "\n";
//            else if (isa<AllocaInst>(I))
//                errs() << "\t\t\tInstruction (type is AllocaInst):" << "\n";
//
//            errs() << "\t\t\t\t" << I << "\n";
//        }
    }
    return PreservedAnalyses::all();
}

bool registerPipeline(StringRef Name, FunctionPassManager &FPM,
                      ArrayRef<PassBuilder::PipelineElement>) {
    if (Name == "BytecodeToDotPass") {
        FPM.addPass(llvm::BytecodeToDotPass());
        return true;
    }
    return false;
}

extern "C" LLVM_ATTRIBUTE_WEAK PassPluginLibraryInfo llvmGetPassPluginInfo() {
    return {LLVM_PLUGIN_API_VERSION, "BytecodeToDotPass", LLVM_VERSION_STRING,
            [](PassBuilder &PB) {
                PB.registerPipelineParsingCallback(registerPipeline);
            }};
}
