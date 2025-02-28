#include "ModuloModel.hpp"

#include "IntegerData.hpp"

#include <QtGui/QDoubleValidator>

ModuloModel::ModuloModel() : NodeDataModel(), _result(std::make_shared<IntegerData>())
{
}

QString ModuloModel::portCaption(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    switch (portType)
    {
        case PortType::In:
            if (portIndex == 0)
                return QStringLiteral("Dividend");
            else if (portIndex == 1)
                return QStringLiteral("Divisor");

            break;

        case PortType::Out: return QStringLiteral("Result");

        default: break;
    }
    return QString();
}

QJsonObject ModuloModel::save() const
{
    QJsonObject modelJson;

    modelJson["name"] = name();

    return modelJson;
}

unsigned int ModuloModel::nPorts(PortType portType) const
{
    unsigned int result = 1;

    switch (portType)
    {
        case PortType::In: result = 2; break;

        case PortType::Out: result = 1;

        default: break;
    }

    return result;
}

NodeDataType ModuloModel::dataType(PortType, PortIndex) const
{
    return IntegerData().type();
}

std::shared_ptr<NodeData> ModuloModel::outData(PortIndex)
{
    return _result;
}

void ModuloModel::setInData(std::shared_ptr<NodeData> data, PortIndex portIndex)
{
    auto numberData = std::dynamic_pointer_cast<IntegerData>(data);

    if (portIndex == 0)
    {
        _number1 = numberData;
    }
    else
    {
        _number2 = numberData;
    }

    compute();
}

NodeValidationState ModuloModel::validationState() const
{
    return modelValidationState;
}

QString ModuloModel::validationMessage() const
{
    return modelValidationError;
}

void ModuloModel::compute()
{
    auto n1 = _number1.lock();
    auto n2 = _number2.lock();

    if (n1 && n2 && n1->isValid() && n2->isValid())
    {
        if (n2 && (n2->number() == 0.0))
        {
            modelValidationState = NodeValidationState::Error;
            modelValidationError = QStringLiteral("Division by zero error");
            _result = std::make_shared<IntegerData>();
        }
        else
        {
            modelValidationState = NodeValidationState::Valid;
            modelValidationError = QString();
            _result = std::make_shared<IntegerData>(n1->number() % n2->number());
        }
    }
    else
    {
        modelValidationState = NodeValidationState::Warning;
        modelValidationError = QStringLiteral("Missing or incorrect inputs");
        _result = std::make_shared<IntegerData>();
    }

    Q_EMIT dataUpdated(0);
}
