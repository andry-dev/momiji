#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include <qnamespace.h>
#include <qpixmap.h>

#include <momiji/config.h>

AboutDialog::AboutDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    // setFixedSize(size());
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

    ui->logo->setPixmap(QPixmap { ":/icons/program.ico" });
    ui->info->setText(
        "<p><b>momiji-qt</b></p>"
        "<br/>" +
        tr("<p>libmomiji version: ") +
        QString::fromStdString(std::string { momiji::versionStr }) +
        "</p><br/>" +
        R"(<p><a href="https://github.com/andry-dev/momiji">)"
        R"(<span style="text-decoration: underline; color:#2980b9;">)"
        R"(Source code)"
        R"(</span></a></p>)");
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
