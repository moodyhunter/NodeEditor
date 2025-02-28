#include "AccumulatorModel.hpp"
#include "AdditionModel.hpp"
#include "Converters.hpp"
#include "DivisionModel.hpp"
#include "FibonacciModel.hpp"
#include "MaxModel.hpp"
#include "MinModel.hpp"
#include "ModuloModel.hpp"
#include "MultiplicationModel.hpp"
#include "NumberDisplayDataModel.hpp"
#include "NumberSourceDataModel.hpp"
#include "SubtractionModel.hpp"

#include <QtWidgets/QApplication>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QVBoxLayout>
#include <nodes/ConnectionStyle>
#include <nodes/DataModelRegistry>
#include <nodes/FlowScene>
#include <nodes/FlowView>
#include <nodes/NodeData>
#include <nodes/TypeConverter>

using QtNodes::ConnectionStyle;
using QtNodes::DataModelRegistry;
using QtNodes::FlowScene;
using QtNodes::FlowView;
using QtNodes::TypeConverter;
using QtNodes::TypeConverterId;

static std::shared_ptr<DataModelRegistry> registerDataModels()
{
    auto ret = std::make_shared<DataModelRegistry>();
    ret->registerModel<NumberSourceDataModel>("I/O//Sources");
    ret->registerModel<NumberDisplayDataModel>("I/O//Displays");

    ret->registerModel<AdditionModel>("Operators");
    ret->registerModel<SubtractionModel>("Operators");
    ret->registerModel<MultiplicationModel>("Operators");
    ret->registerModel<DivisionModel>("Operators");
    ret->registerModel<ModuloModel>("Operators");

    ret->registerModel<AccumulatorModel>("Special");
    ret->registerModel<MinModel>("Special");
    ret->registerModel<MaxModel>("Special");
    ret->registerModel<FibonacciModel>("Special");

    ret->registerTypeConverter(std::make_pair(DecimalData().type(), IntegerData().type()), TypeConverter{ DecimalToIntegerConverter() });

    ret->registerTypeConverter(std::make_pair(IntegerData().type(), DecimalData().type()), TypeConverter{ IntegerToDecimalConverter() });

    return ret;
}

static void setStyle()
{
    ConnectionStyle::setConnectionStyle(
        R"(
  {
    "ConnectionStyle": {
      "ConstructionColor": "gray",
      "NormalColor": "black",
      "SelectedColor": "gray",
      "SelectedHaloColor": "deepskyblue",
      "HoveredColor": "deepskyblue",

      "LineWidth": 3.0,
      "ConstructionLineWidth": 2.0,
      "PointDiameter": 10.0,

      "UseDataDefinedColors": true
    }
  }
  )");
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    setStyle();

    QWidget mainWidget;

    auto menuBar = new QMenuBar();
    auto saveAction = menuBar->addAction("Save");
    auto loadAction = menuBar->addAction("Load");

    QVBoxLayout *l = new QVBoxLayout(&mainWidget);

    l->addWidget(menuBar);
    auto scene = new FlowScene(registerDataModels(), &mainWidget);
    l->addWidget(new FlowView(scene));
    l->setContentsMargins(0, 0, 0, 0);
    l->setSpacing(0);

    QObject::connect(saveAction, &QAction::triggered, scene, &FlowScene::save);

    QObject::connect(loadAction, &QAction::triggered, scene, &FlowScene::load);

    mainWidget.setWindowTitle("Dataflow tools: simplest calculator");
    mainWidget.resize(800, 600);
    mainWidget.showNormal();

    return app.exec();
}
