#include "VFileSelector.h"

#include "FileOperationChecker.h"
#include "ErrorReporter.h"

#include "VPushButton.h"
#include "VLineEdit.h"

VFileSelector2::VFileSelector2(const std::string &buttonText, const std::string &defaultPath, const std::string &filter = "") : VContainer(this), _filePath(defaultPath), _filter(filter)
{
    _pushButton = new VPushButton2(buttonText);
    _lineEdit = new VLineEdit2(defaultPath);
    layout()->addWidget(_pushButton);
    layout()->addWidget(_lineEdit);

    if (_filePath.empty()) _filePath = QDir::homePath().toStdString();

    connect(_pushButton, SIGNAL(ButtonClicked()), this, SLOT(OpenFileDialog()));
    connect(_lineEdit, SIGNAL(ValueChanged(const std::string &)), this, SLOT(SetPathFromLineEdit(const std::string &)));
}

std::string VFileSelector2::GetValue() const { return _filePath; }

bool VFileSelector2::SetValue(const std::string &file)
{
    bool success = false;

    if (file == _filePath)    // Do nothing, if nothing is changed
        return success;

    if (_isFileOperable(file)) {
        _filePath = file;
        success = true;
    } else {
        MSG_ERR(FileOperationChecker::GetLastErrorMessage().toStdString());
    }

    _lineEdit->SetValue(_filePath);
    return success;
}

void VFileSelector2::OpenFileDialog()
{
    std::string file = _launchFileDialog();
    if (SetValue(file)) emit ValueChanged(_filePath);
}

void VFileSelector2::SetPathFromLineEdit(const std::string &file)
{
    if (SetValue(file)) emit ValueChanged(_filePath);
}

//
//
//
VFileReader2::VFileReader2(const std::string &buttonText, const std::string &defaultPath, const std::string &filter) : VFileSelector2(buttonText, defaultPath, filter) {}

std::string VFileReader2::_launchFileDialog()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Select Directory/File", QString::fromStdString(_filePath), QString::fromStdString(_filter));
    return fileName.toStdString();
}

bool VFileReader2::_isFileOperable(const std::string &filePath) const
{
    bool operable = false;
    operable = FileOperationChecker::FileGoodToRead(QString::fromStdString(filePath));
    return operable;
}

//
//
//
VFileWriter2::VFileWriter2(const std::string &buttonText, const std::string &defaultPath, const std::string &filter) : VFileSelector2(buttonText, defaultPath, filter) {}

bool VFileWriter2::_isFileOperable(const std::string &filePath) const
{
    bool operable = false;
    operable = FileOperationChecker::FileGoodToWrite(QString::fromStdString(filePath));
    return operable;
}

std::string VFileWriter2::_launchFileDialog()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Select Directory/File", QString::fromStdString(_filePath), QString::fromStdString(_filter));
    return fileName.toStdString();
}

VDirSelector::VDirSelector(const std::string &buttonText, const std::string &defaultPath) : VFileSelector2(buttonText, defaultPath) {}

std::string VDirSelector::_launchFileDialog()
{
    QString fileName = QFileDialog::getExistingDirectory(this, "Select Directory/File", QString::fromStdString(_filePath));
    return fileName.toStdString();
}

bool VDirSelector::_isFileOperable(const std::string &filePath) const
{
    bool operable = false;
    operable = FileOperationChecker::DirectoryGoodToRead(QString::fromStdString(filePath));
    return operable;
}
