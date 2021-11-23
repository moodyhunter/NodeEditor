#pragma once

#include <QtWidgets/QLabel>
#include <nodes/NodeDataModel>

using QtNodes::NodeData;
using QtNodes::NodeDataModel;
using QtNodes::NodeDataType;
using QtNodes::NodeValidationState;
using QtNodes::PortIndex;
using QtNodes::PortType;

/// The model dictates the number of inputs and outputs for the Node.
/// In this example it has no logic.
class ImageShowModel : public NodeDataModel
{
    Q_OBJECT

  public:
    ImageShowModel();

  public:
    QString caption() const override
    {
        return QString("Image Display");
    }

    QString name() const override
    {
        return QString("ImageShowModel");
    }

  public:
    virtual QString modelName() const
    {
        return QString("Resulting Image");
    }

    unsigned int nPorts(PortType portType) const override;

    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;

    std::shared_ptr<NodeData> outData(PortIndex port) override;

    void setInData(std::shared_ptr<NodeData> nodeData, PortIndex port) override;

    QWidget *embeddedWidget() override;

    bool resizable() const override
    {
        return true;
    }

  protected:
    bool eventFilter(QObject *object, QEvent *event) override;

  signals:

    void updateLabel(const QPixmap &);

  private:
    QLabel *_label;

    std::shared_ptr<NodeData> _nodeData;
};
