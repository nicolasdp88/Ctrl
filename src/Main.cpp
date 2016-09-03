#include "Node.h"

void main()
{

	auto buf_ptr = std::shared_ptr<int>(new int[10], std::default_delete<int[]>());

	Variable<std::shared_ptr<int>> IntArrayVar;


	Variable<std::vector<int>> testVec;

	testVec().push_back(1);
	testVec().push_back(1);
	testVec().push_back(1);
	testVec().push_back(1);
	testVec().push_back(1);
	testVec().push_back(1);
	testVec().push_back(7);
	testVec().push_back(7);
	testVec().push_back(10);


	IntVariable testVar(5);	
	IntVariable testVar2(5);

	auto test = testVec();
	auto vecCopy(testVec);

	auto vecCopyValue = vecCopy();
	auto& vecRefValue = vecCopy();
	auto varCopy = vecCopy;

	auto varCopyValue = varCopy();

	//varCopy().push_back(11);

	std::cout << (testVec() == varCopy()) << std::endl;

	//std::cout << testVar() << std::cout;

	AddOperation<int> op;
	
	VariableBasePtr ABase = op.inputs().at("A");

	IntVariablePtr ABaseRef = std::dynamic_pointer_cast<IntVariable>(ABase);

	*std::dynamic_pointer_cast<IntVariable>(op.inputs().at("A")) = testVar;
	*std::dynamic_pointer_cast<IntVariable>(op.inputs().at("B")) = testVar2;

	auto result = std::dynamic_pointer_cast<IntVariable>(op.outputs().at("Result"));
	auto res = (*result)();

	std::cout << res << std::endl;

	PrintFunction printFct;

	StringVariablePtr printVar = std::dynamic_pointer_cast<StringVariable>(printFct.inputs().at("ToPrint"));
	*std::dynamic_pointer_cast<StringVariable>(printFct.inputs().at("ToPrint")) = std::to_string(res);

	printFct.exec();
}