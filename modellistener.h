#pragma once

class ModelListener
{
public:
    explicit ModelListener() {}
    virtual ~ModelListener() {}
    virtual void updateView() = 0;
};
