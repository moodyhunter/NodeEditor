#pragma once

#include "TextData.hpp"

#include <QtWidgets/QLabel>
#include <nodes/NodeDataModel>

using QtNodes::NodeData;
using QtNodes::NodeDataModel;
using QtNodes::PortIndex;
using QtNodes::PortType;

/// The model dictates the number of inputs and outputs for the Node.
/// In this example it has no logic.
class TextDisplayDataModel : public NodeDataModel
{
    Q_OBJECT

  public:
    TextDisplayDataModel();

  public:
    QString caption() const override
    {
        return QString("Text Display");
    }

    bool captionVisible() const override
    {
        return false;
    }

    static QString Name()
    {
        return QString("TextDisplayDataModel");
    }

    QString name() const override
    {
        return TextDisplayDataModel::Name();
    }

  public:
    unsigned int nPorts(PortType portType) const override;

    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;

    std::shared_ptr<NodeData> outData(PortIndex port) override;

    void setInData(std::shared_ptr<NodeData> data, int) override;

    QWidget *embeddedWidget() override;

  signals:

    void updateLabel(QString);

  private:
    QLabel *_label;
};
