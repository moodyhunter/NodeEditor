#pragma once

#include <QtWidgets/QLabel>
#include <iostream>
#include <nodes/NodeDataModel>

using QtNodes::NodeData;
using QtNodes::NodeDataModel;
using QtNodes::NodeDataType;
using QtNodes::NodeValidationState;
using QtNodes::PortIndex;
using QtNodes::PortType;

class NumberDisplayDataModel : public NodeDataModel
{
    Q_OBJECT

  public:
    NumberDisplayDataModel();

  public:
    QString caption() const override
    {
        return QStringLiteral("Result");
    }

    bool captionVisible() const override
    {
        return false;
    }

    QString name() const override
    {
        return QStringLiteral("Result");
    }

  public:
    unsigned int nPorts(PortType portType) const override;

    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;

    std::shared_ptr<NodeData> outData(PortIndex port) override;

    void setInData(std::shared_ptr<NodeData> data, int) override;

    QWidget *embeddedWidget() override;

    NodeValidationState validationState() const override;

    QString validationMessage() const override;

  signals:
    void updateLabel(QString);

  private:
    NodeValidationState modelValidationState = NodeValidationState::Warning;
    QString modelValidationError = QStringLiteral("Missing or incorrect inputs");

    QLabel *_label;
};
