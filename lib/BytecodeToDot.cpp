#include "../include/BytecodeToDot.h"

using namespace llvm;

PreservedAnalyses BytecodeToDotPass::run(Function &F,
                                         FunctionAnalysisManager &AM) {
    std::string module_name = F.getParent()->getName().str();
    std::string dotfilename = module_name + "_" + F.getNameOrAsOperand() + ".dot";

    if (std::filesystem::exists(dotfilename))
        remove(dotfilename.c_str());

    std::ofstream dotfile;
    dotfile.open(dotfilename, std::ios_base::app);
    dotfile << "digraph \"CFG for '" << F.getNameOrAsOperand() << "' function\"{\n";
    for (BasicBlock &BB : F) {
        for (BasicBlock *pred : predecessors(&BB))
            dotfile << "Node_" << pred->getValueName() << " -> " << "Node_" << BB.getValueName() << ";\n";

        dotfile << "Node_" << BB.getValueName() << " [shape=record,label=\"{";
        dotfile << BB.getName().str() << ":\\l\\l";
        for (Instruction &I : BB) {
            dotfile << " ";
            if (I.getNameOrAsOperand() != "<badref>") {
                if (I.hasName())
                    dotfile << "%";
                dotfile << I.getNameOrAsOperand() << " = ";
            }
            dotfile << I.getOpcodeName() << " ";

            size_t numOperands = I.getNumOperands();
            if (isa<CallInst>(I)) {
                std::string funcName = I.getOperand(--numOperands)->getNameOrAsOperand();
                dotfile << "@" << funcName << "(";
                for (Use &U: I.operands()) {
                    Value *val = U.get();
                    if (val->getNameOrAsOperand() == funcName) {
                        continue;
                    }
                    if (val->hasName())
                        dotfile << "%";
                    dotfile << val->getNameOrAsOperand();
                    if (--numOperands != 0)
                        dotfile << ", ";
                }
                dotfile << ")";
            } else if (auto *PN = dyn_cast<PHINode>(&I)) {
                for (auto &bb : PN->blocks()) {
                    Value *val = PN->getIncomingValueForBlock(bb);
                    dotfile << "[ ";
                    if (val->hasName())
                        dotfile << "%";
                    dotfile << val->getNameOrAsOperand() << ", ";
                    if (bb->hasName())
                        dotfile << "%";
                    dotfile << bb->getNameOrAsOperand() << " ]";
                    if (--numOperands != 0)
                        dotfile << ", ";
                }
            } else {
                for (Use &U : I.operands()) {
                    Value *val = U.get();
                    if (val->hasName())
                        dotfile << "%";
                    dotfile << val->getNameOrAsOperand();
                    if (--numOperands != 0)
                        dotfile << ", ";
                }
            }
            dotfile << "\\l";
        }
        dotfile << "}\"];\n";
    }
    dotfile << "}\n";
    dotfile.close();

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
