#include "SubtractionModel.hpp"

#include "DecimalData.hpp"

QString SubtractionModel::portCaption(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    switch (portType)
    {
        case PortType::In:
            if (portIndex == 0)
                return QStringLiteral("Minuend");
            else if (portIndex == 1)
                return QStringLiteral("Subtrahend");

            break;

        case PortType::Out: return QStringLiteral("Result");

        default: break;
    }
    return QString();
}

void SubtractionModel::compute()
{
    auto n1 = _number1.lock();
    auto n2 = _number2.lock();

    if (n1 && n2)
    {
        setValidState();
        _result = std::make_shared<DecimalData>(n1->number() - n2->number());
    }
    else
    {
        setWarningState();
        _result = std::make_shared<DecimalData>();
    }

    Q_EMIT dataUpdated(0);
}
