#include "sidebarbutton.h"
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QSizePolicy>

SideBarButton::SideBarButton(QWidget* parent) :
    QWidget(parent),
    actionOwner(nullptr)
{
    QHBoxLayout *hLayout = new QHBoxLayout(this);

    indicatorLabel = new QLabel(this);
    indicatorLabel->setProperty("type", 42);
    indicatorLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    indicatorLabel->setVisible(false);
    QSizePolicy indicatorRetainSize = indicatorLabel->sizePolicy();
    indicatorRetainSize.setRetainSizeWhenHidden(true);
    indicatorLabel->setSizePolicy(indicatorRetainSize);

    textLabel = new QLabel(this);
    textLabel->setProperty("type", 43);
    textLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    batchLabel = new QLabel(this);
    batchLabel->setProperty("type", 44);
    batchLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    batchLabel->setVisible(false);
    QSizePolicy batchRetainSize = batchLabel->sizePolicy();
    batchRetainSize.setRetainSizeWhenHidden(true);
    batchLabel->setSizePolicy(batchRetainSize);

    hLayout->addWidget(indicatorLabel);
    hLayout->addWidget(textLabel);
    hLayout->addWidget(batchLabel);
    hLayout->addStretch(1);
    hLayout->setSpacing(0);
    hLayout->setMargin(0);
    hLayout->setContentsMargins(0,0,0,0);
}

void SideBarButton::setAction(QAction* action)
{
    // if I've got already an action associated to the button
    // remove all connections
    if ( actionOwner && actionOwner != action ) {
        disconnect(actionOwner, &QAction::changed,
                   this, &SideBarButton::updateButtonStatusFromAction);
        disconnect(this, &SideBarButton::clicked,
                   actionOwner, &QAction::trigger);
    }

    // store the action
    actionOwner = action;

    // configure the button
    updateButtonStatusFromAction();

    // connect the action and the button
    // so that when the action is changed the
    // button is changed too!
    connect(action, &QAction::changed,
            this, &SideBarButton::updateButtonStatusFromAction);

    // connect the button to the slot that forwards the
    // signal to the action
    connect(this, &SideBarButton::clicked,
            actionOwner, &QAction::trigger);
}

void SideBarButton::updateButtonStatusFromAction()
{
    if(!actionOwner)
        return;
    setText( actionOwner->iconText() );
    //setStatusTip( actionOwner->statusTip() );
    //setToolTip( actionOwner->toolTip() );
    //setEnabled( actionOwner->isEnabled() );
    //setCheckable( actionOwner->isCheckable() );
    //setChecked( actionOwner->isChecked());
    indicatorLabel->setVisible(actionOwner->isChecked());
}

void SideBarButton::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        emit clicked();
    }
}

QSize SideBarButton::sizeHint() const
{
    ensurePolished();
    int w = 0;
    int h = 0;
    w = indicatorLabel->sizeHint().rwidth() + textLabel->sizeHint().rwidth() + batchLabel->sizeHint().rwidth();
    h = std::max({indicatorLabel->sizeHint().rheight(),textLabel->sizeHint().rheight(),batchLabel->sizeHint().rheight()});
    return QSize(w,h);
}

void SideBarButton::setText(const QString &text)
{
    textLabel->setText(text);
}

void SideBarButton::setBatchText(const QString &text)
{
    batchLabel->setText(text);
}

void SideBarButton::activateBatch()
{
    batchLabel->setVisible(true);
}

void SideBarButton::deactivateBatch()
{
    batchLabel->setVisible(false);
}

