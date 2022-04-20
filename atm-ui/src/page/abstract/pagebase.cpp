#include "page/abstract/pagebase.h"

PageBase::PageBase(StateManager *stateManager, QWidget *parent) : QWidget{parent} {
    this->stateManager = stateManager;
}

PageBase::~PageBase() {

}
