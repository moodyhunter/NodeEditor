#pragma once

#include "DecimalData.hpp"
#include "IntegerData.hpp"

#include <nodes/TypeConverter>

using QtNodes::NodeData;
using QtNodes::NodeDataType;

class DecimalToIntegerConverter
{

  public:
    std::shared_ptr<NodeData> operator()(std::shared_ptr<NodeData> data);

  private:
    std::shared_ptr<NodeData> _integer;
};

class IntegerToDecimalConverter
{

  public:
    std::shared_ptr<NodeData> operator()(std::shared_ptr<NodeData> data);

  private:
    std::shared_ptr<NodeData> _decimal;
};
