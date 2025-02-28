#pragma once

#include "DecimalData.hpp"

#include <nodes/Connection>
#include <nodes/NodeDataModel>

using QtNodes::NodeData;
using QtNodes::NodeDataModel;
using QtNodes::NodeDataType;
using QtNodes::NodeValidationState;
using QtNodes::PortIndex;
using QtNodes::PortType;

/*
 * This model has the purpose of showing a node with dynamic output ports.
 */

class FibonacciModel : public NodeDataModel
{
    Q_OBJECT

  public:
    FibonacciModel();

    unsigned int nPorts(PortType portType) const override;

    bool hasDynamicPorts(PortType portType) const override;

    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;

    std::shared_ptr<NodeData> outData(PortIndex port) override;

    void setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) override;

  public:
    QString caption() const override;

    bool portCaptionVisible(PortType portType, PortIndex portIndex) const override;

    QString portCaption(PortType portType, PortIndex portIndex) const override;

    QString name() const override;

  public:
    QWidget *embeddedWidget() override
    {
        return nullptr;
    }

    NodeValidationState validationState() const override;

    QString validationMessage() const override;

  private:
    void restore(const QJsonObject &obj) override;

    void outputConnectionCreated(const QtNodes::Connection &c) override;

    void outputConnectionDeleted(const QtNodes::Connection &c) override;

  private:
    int _indexCounter;

    NodeValidationState _modelValidationState = NodeValidationState::Warning;
    QString _modelValidationError = QString("Missing or incorrect inputs");
};
