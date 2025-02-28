#pragma once

#include "Export.hpp"
#include "NodeData.hpp"
#include "NodeGeometry.hpp"
#include "NodePainterDelegate.hpp"
#include "NodeStyle.hpp"
#include "PortType.hpp"
#include "Serializable.hpp"
#include "memory.hpp"

#include <QtWidgets/QWidget>

namespace QtNodes
{

    enum class NodeValidationState
    {
        Valid,
        Warning,
        Error
    };

    class Connection;

    class StyleCollection;

    class NODE_EDITOR_PUBLIC NodeDataModel
        : public QObject
        , public Serializable
    {
        Q_OBJECT

      public:
        NodeDataModel();

        virtual ~NodeDataModel() = default;

        /// Caption is used in GUI
        virtual QString caption() const = 0;

        /// It is possible to hide caption in GUI
        virtual bool captionVisible() const
        {
            return true;
        }

        /// Port caption is used in GUI to label individual ports
        virtual QString portCaption(PortType, PortIndex) const
        {
            return QString();
        }

        /// It is possible to hide port caption in GUI
        virtual bool portCaptionVisible(PortType, PortIndex) const
        {
            return false;
        }

        /// Name makes this model unique
        virtual QString name() const = 0;

      public:
        QJsonObject save() const override;

      public:
        virtual unsigned int nPorts(PortType portType) const = 0;

        /// Return if ports are dynamics
        virtual bool hasDynamicPorts(PortType) const
        {
            return false;
        }

        virtual NodeDataType dataType(PortType portType, PortIndex portIndex) const = 0;

      public:
        enum class ConnectionPolicy
        {
            One,
            Many
        };

        ConnectionPolicy portConnectionPolicy(PortType portType, PortIndex portIndex) const;

        virtual ConnectionPolicy portOutConnectionPolicy(PortIndex) const
        {
            return ConnectionPolicy::Many;
        }

        virtual ConnectionPolicy portInConnectionPolicy(PortIndex) const
        {
            return ConnectionPolicy::One;
        }

        NodeStyle const &nodeStyle() const;

        void setNodeStyle(NodeStyle const &style);

      public:
        /// Triggers the algorithm
        virtual void setInData(std::shared_ptr<NodeData> nodeData, PortIndex port);

        virtual void setInData(std::vector<std::shared_ptr<NodeData>> nodeData, PortIndex port);

        virtual std::shared_ptr<NodeData> outData(PortIndex port) = 0;

        virtual QWidget *embeddedWidget() = 0;

        virtual bool resizable() const
        {
            return false;
        }

        virtual NodeValidationState validationState() const
        {
            return NodeValidationState::Valid;
        }

        virtual QString validationMessage() const
        {
            return QString("");
        }

        virtual NodePainterDelegate *painterDelegate() const
        {
            return nullptr;
        }

      public Q_SLOTS:

        virtual void inputConnectionCreated(Connection const &)
        {
        }

        virtual void inputConnectionDeleted(Connection const &)
        {
        }

        virtual void outputConnectionCreated(Connection const &)
        {
        }

        virtual void outputConnectionDeleted(Connection const &)
        {
        }

      Q_SIGNALS:

        void dataUpdated(PortIndex index);

        void dataInvalidated(PortIndex index);

        void computingStarted();

        void computingFinished();

        void embeddedWidgetSizeUpdated();

        void portAdded(PortType type, PortIndex index);

        void portMoved(PortType type, PortIndex oldIndex, PortIndex newIndex);

        void portRemoved(PortType type, PortIndex index);

      private:
        NodeStyle _nodeStyle;
    };
} // namespace QtNodes
