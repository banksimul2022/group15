#ifndef PININTERFACE_H
#define PININTERFACE_H

#include "Pin_global.h"
#include <QWidget>
#include <QString>
#include <QDebug>
#include <QTimer>

class PIN_EXPORT PinInterface : public QWidget
{
    Q_OBJECT
public:
    static PinInterface *createWidgetInstance(QWidget *parent = nullptr);
    explicit PinInterface(QWidget *parent = nullptr);
    virtual ~PinInterface() = 0;
    virtual void setPinMessage(QString message) = 0;

signals:
    void pinWidgetUserInput(QString userInput);
    void deletePinWidget();


};
#endif // PININTERFACE_H
