#include <optional>

#include "view_calc.hpp"
#include "./ui_view_calc.h"

CalcView::CalcView(CalcController* controller, QWidget *parent) :
    QMainWindow(parent),
    controller_(controller),
    ui_(std::make_unique<Ui::CalcView>())
{
    ui_->setupUi(this);
    this->setFixedSize(480, 380);

    QVector<QPushButton*> buttons({
        ui_->btnZero, ui_->btnOne,  ui_->btnTwo,   ui_->btnThree, ui_->btnFour,
        ui_->btnFive, ui_->btnSix,  ui_->btnSeven, ui_->btnEight, ui_->btnNine,
        ui_->btnX,    ui_->btnPi,   ui_->btnDot,   ui_->btnLB,    ui_->btnRB,
        ui_->btnCos,  ui_->btnAcos, ui_->btnSin,   ui_->btnAsin,  ui_->btnTan,
        ui_->btnAtan, ui_->btnAbs,  ui_->btnSqrt,  ui_->btnLog,   ui_->btnLn,
        ui_->btnPow,  ui_->btnMod,  ui_->btnMinus, ui_->btnPlus,  ui_->btnMul,
        ui_->btnDiv
    });

    for (auto button : buttons)
        connect(button, &QPushButton::clicked, this, &CalcView::NumsAndFuncsClicked);

    connect(ui_->btnRad, &QPushButton::clicked, this, &CalcView::BtnRadClicked);
    connect(ui_->btnDeg, &QPushButton::clicked, this, &CalcView::BtnDegClicked);
    connect(ui_->btnDel, &QPushButton::clicked, this, &CalcView::BtnDelClicked);
    connect(ui_->btnGraph, &QPushButton::clicked, this, &CalcView::BtnGraphClicked);
    connect(ui_->btnDelAll, &QPushButton::clicked, this, &CalcView::BtnDelAllClicked);
    connect(ui_->btnResult, &QPushButton::clicked, this, &CalcView::BtnResultClicked);
    connect(ui_->btnShowGraph, &QPushButton::clicked, this, &CalcView::BtnShowGraphClicked);
}

CalcView::~CalcView() {}

void CalcView::BtnResultClicked() {
    std::string input{ui_->leInput->text().toStdString()};
    std::optional<double> result{controller_->Calculate(input)};

    if (result.has_value()) {
        ui_->leOutput->setText(QString::number(*result));
    } else {
        ui_->leOutput->setText(QString("ERROR: Icorrect data!"));
    }
}

void CalcView::NumsAndFuncsClicked() {
    QPushButton* button{static_cast<QPushButton*>(sender())};
    QString button_text{button->text()};
    QString current_text{ui_->leInput->text()};

    ui_->leInput->setText(current_text + button_text);
    token_sizes_.push(static_cast<std::uint8_t>(button_text.size()));
}

void CalcView::BtnDelClicked() {
    if (!token_sizes_.empty()) {
        QString text{ui_->leInput->text()};

        text.chop(token_sizes_.top());
        ui_->leInput->setText(text);
        token_sizes_.pop();
    }
}

void CalcView::BtnDelAllClicked() {
    ui_->leInput->clear();
    ui_->leOutput->clear();

    while(!token_sizes_.empty())
        token_sizes_.pop();
}

void CalcView::BtnRadClicked() {
    ui_->btnRad->setStyleSheet(
        "background-color: rgb(255, 160, 122); "
        "color: black; "
        "border: 1px solid gray;"
        );

    ui_->btnDeg->setStyleSheet(
        "background-color: rgb(255, 219, 139); "
        "color: black; "
        "border: 1px solid gray;"
        );
}

void CalcView::BtnDegClicked() {
    ui_->btnDeg->setStyleSheet(
        "background-color: rgb(255, 160, 122); "
        "color: black; "
        "border: 1px solid gray;"
    );

    ui_->btnRad->setStyleSheet(
        "background-color: rgb(255, 219, 139); "
        "color: black; "
        "border: 1px solid gray;"
    );
}

void CalcView::BtnShowGraphClicked() {
    bool graph_is_open{ui_->btnShowGraph->text() != ">"};

    if (!graph_is_open) {
        this->setFixedSize(960, 380);
        ui_->btnShowGraph->setText("<");
    } else {
        this->setFixedSize(480, 380);
        ui_->btnShowGraph->setText(">");
    }
}

void CalcView::BtnGraphClicked() {
    if (ui_->btnShowGraph->text() == ">") {
        BtnShowGraphClicked();
    }

    PrintGraph();
}

void CalcView::PrintGraph() {
    ui_->qcpGraph->clearGraphs();

    ui_->qcpGraph->xAxis->setRange(
        ui_->leMinX->text().toDouble(),
        ui_->leMaxX->text().toDouble()
    );

    ui_->qcpGraph->yAxis->setRange(
        ui_->leMinY->text().toDouble(),
        ui_->leMaxY->text().toDouble()
    );

    QVector<double> x_cords;
    QVector<double> y_cords;

    double x{ui_->leStartX->text().toDouble()};
    double x_end{ui_->leEndX->text().toDouble() + 0.1};

    while (x <= x_end) {
        x_cords.push_back(x);
//        y_cords.push_back(*controller_->Calculate(input));
        x += 0.1;
    }

    ui_->qcpGraph->addGraph();
    ui_->qcpGraph->graph(0)->addData(x_cords, y_cords);
    ui_->qcpGraph->replot();
}
