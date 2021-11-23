#pragma once

#include <nodes/NodeDataModel>

class DecimalData;

using QtNodes::NodeData;
using QtNodes::NodeDataModel;
using QtNodes::NodeDataType;
using QtNodes::NodeValidationState;
using QtNodes::PortIndex;
using QtNodes::PortType;

/// The model dictates the number of inputs and outputs for the Node.
/// In this example it has no logic.
class MathOperationDataModel : public NodeDataModel
{
    Q_OBJECT

  public:
    MathOperationDataModel();

  public:
    unsigned int nPorts(PortType portType) const override;

    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;

    std::shared_ptr<NodeData> outData(PortIndex port) override;

    void setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) override;

    QWidget *embeddedWidget() override
    {
        return nullptr;
    }

    NodeValidationState validationState() const override;

    QString validationMessage() const override;

  protected:
    virtual void compute() = 0;

    void setWarningState();

    void setValidState();

    void setErrorState(const QString &msg = "");

  protected:
    std::weak_ptr<DecimalData> _number1;
    std::weak_ptr<DecimalData> _number2;

    std::shared_ptr<DecimalData> _result;

  private:
    NodeValidationState _modelValidationState = NodeValidationState::Warning;
    QString _modelValidationError = QString("Missing or incorrect inputs");
};
