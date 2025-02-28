#include "ConnectionGraphicsObject.hpp"

#include "Connection.hpp"
#include "ConnectionBlurEffect.hpp"
#include "ConnectionGeometry.hpp"
#include "ConnectionPainter.hpp"
#include "ConnectionState.hpp"
#include "FlowScene.hpp"
#include "Node.hpp"
#include "NodeConnectionInteraction.hpp"
#include "NodeGraphicsObject.hpp"

#include <QtWidgets/QGraphicsBlurEffect>
#include <QtWidgets/QGraphicsDropShadowEffect>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QStyleOptionGraphicsItem>

using QtNodes::Connection;
using QtNodes::ConnectionGraphicsObject;
using QtNodes::FlowScene;

ConnectionGraphicsObject::ConnectionGraphicsObject(FlowScene &scene, Connection &connection) : _scene(scene), _connection(connection)
{
    _scene.addItem(this);

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    setAcceptHoverEvents(true);

    // addGraphicsEffect();

    setZValue(-1.0);
}

ConnectionGraphicsObject::~ConnectionGraphicsObject()
{
    _scene.removeItem(this);
}

QtNodes::Connection &ConnectionGraphicsObject::connection()
{
    return _connection;
}

QRectF ConnectionGraphicsObject::boundingRect() const
{
    return _connection.connectionGeometry().boundingRect();
}

QPainterPath ConnectionGraphicsObject::shape() const
{
#ifdef DEBUG_DRAWING

    // QPainterPath path;

    // path.addRect(boundingRect());
    // return path;

#else
    auto const &geom = _connection.connectionGeometry();

    return ConnectionPainter::getPainterStroke(geom);

#endif
}

void ConnectionGraphicsObject::setGeometryChanged()
{
    prepareGeometryChange();
}

void ConnectionGraphicsObject::move()
{
    for (PortType portType : { PortType::In, PortType::Out })
    {
        if (auto node = _connection.getNode(portType))
        {
            auto const &nodeGraphics = node->nodeGraphicsObject();

            auto const &nodeGeom = node->nodeGeometry();

            QPointF scenePos = nodeGeom.portScenePosition(_connection.getPortIndex(portType), portType, nodeGraphics.sceneTransform());

            QTransform sceneTransform = this->sceneTransform();

            QPointF connectionPos = sceneTransform.inverted().map(scenePos);

            _connection.connectionGeometry().setEndPoint(portType, connectionPos);

            _connection.getConnectionGraphicsObject().setGeometryChanged();
            _connection.getConnectionGraphicsObject().update();
        }
    }
}

void ConnectionGraphicsObject::lock(bool locked)
{
    setFlag(QGraphicsItem::ItemIsMovable, !locked);
    setFlag(QGraphicsItem::ItemIsFocusable, !locked);
    setFlag(QGraphicsItem::ItemIsSelectable, !locked);
}

void ConnectionGraphicsObject::paint(QPainter *painter, QStyleOptionGraphicsItem const *option, QWidget *)
{
    painter->setClipRect(option->exposedRect);

    ConnectionPainter::paint(painter, _connection);
}

void ConnectionGraphicsObject::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
}

void ConnectionGraphicsObject::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    prepareGeometryChange();

    auto view = static_cast<QGraphicsView *>(event->widget());
    auto node = locateNodeAt(event->scenePos(), _scene, view->transform());

    auto &connectionState = _connection.connectionState();

    connectionState.interactWithNode(node);
    if (node)
    {
        auto oppPort = oppositePort(connectionState.requiredPort());
        node->reactToPossibleConnection(connectionState.requiredPort(), _connection.dataType(oppPort), event->scenePos());
    }

    //-------------------

    QPointF offset = event->pos() - event->lastPos();

    auto requiredPort = _connection.requiredPort();

    // This moves the loose end exactly to the mouse position
    if (requiredPort != PortType::None)
    {
        _connection.connectionGeometry().moveEndPoint(requiredPort, offset);
    }

    //-------------------

    update();

    event->accept();
}

void ConnectionGraphicsObject::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    ungrabMouse();
    event->accept();

    auto node = locateNodeAt(event->scenePos(), _scene, _scene.views()[0]->transform());

    NodeConnectionInteraction interaction(*node, _connection, _scene);

    if (node && interaction.tryConnect())
    {
        node->resetReactionToConnection();
    }

    if (_connection.connectionState().requiresPort())
    {
        _scene.deleteConnection(_connection);
    }
}

void ConnectionGraphicsObject::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    _connection.connectionGeometry().setHovered(true);

    update();
    _scene.connectionHovered(connection(), event->screenPos());
    event->accept();
}

void ConnectionGraphicsObject::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    _connection.connectionGeometry().setHovered(false);

    update();
    _scene.connectionHoverLeft(connection());
    event->accept();
}

void ConnectionGraphicsObject::addGraphicsEffect()
{
    auto effect = new QGraphicsBlurEffect;

    effect->setBlurRadius(5);
    setGraphicsEffect(effect);

    // auto effect = new QGraphicsDropShadowEffect;
    // auto effect = new ConnectionBlurEffect(this);
    // effect->setOffset(4, 4);
    // effect->setColor(QColor(Qt::gray).darker(800));
}
