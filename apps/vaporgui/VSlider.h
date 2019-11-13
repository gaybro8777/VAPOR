#pragma once

#include <string>

#include <QWidget>
#include <QSlider>

#include "VContainer.h"

class VSlider2 : public VContainer {
    Q_OBJECT

public:
    VSlider2(double min = 0, double max = 1);

    void SetValue(double value);
    void SetRange(double min, double max);
    void SetIntType(bool isInt);

    double GetValue() const;

private:
    QSlider *_slider;
    double   _minValid;
    double   _maxValid;
    double   _stepSize;
    bool     _isInt;

public slots:
    void emitSliderChanged();
    void emitSliderChangedIntermediate(int position);

signals:
    void ValueChanged(double value);
    void ValueChangedIntermediate(double value);
};
