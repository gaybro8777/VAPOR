#include "VaporWidgets.h"
#include "FileOperationChecker.h"
#include "ErrorReporter.h"

#include <QLabel>
#include <QSlider>
#include <QWidget>
#include <QSpinBox>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QValidator>
#include <QPushButton>
#include <QFileDialog>
#include <QSpacerItem>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <iostream>
#include "vapor/VAssert.h"

#define XMIN 0
#define YMIN 1
#define ZMIN 2
#define XMAX 3
#define YMAX 4
#define ZMAX 5

VaporWidget::VaporWidget(QWidget *parent) : QWidget(parent) {}

void VaporWidget::addToolTip(const std::string &toolTip) { setToolTip(QString::fromStdString(toolTip)); }

/*VLabel::VLabel(
    QWidget* parent,
    const std::string& labelText
    ) :
    VaporWidget( parent )
{
    _layout = new QHBoxLayout(this);
    _layout->setContentsMargins( 0, 0, 0, 0 );

    _label = new QLabel(this);
    _spacer = new QSpacerItem(
        10,
        10,
        QSizePolicy::MinimumExpanding,
        QSizePolicy::Minimum
    );

    _layout->addWidget( _label );
    _layout->addItem( _spacer );  // sets _spacer's parent to _layout

    SetLabelText( labelText );

    setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );
}

VLabel::VLabel(
    const std::string& labelText
    ) :
    VLabel( nullptr, labelText )
{}


void VLabel::SetLabelText( const std::string& text )
{
    _label->setText( QString::fromStdString( text ) );
}*/

/*VSpinBox::VSpinBox(
        QWidget *parent,
        const std::string& labelText,
        int min,
        int max,
        int defaultValue
    ) :
    VLabel(parent, labelText),
    _min( min ),
    _max( max ),
    _value( defaultValue )
{
    _spinBox = new QSpinBox( this );
    SetRange( min, max );

    _spinBox->setValue( _value );

    _layout->addWidget( _spinBox );

    connect( _spinBox, SIGNAL( editingFinished() ),
        this, SLOT( _validateAndEmit() ) );
}

VSpinBox::VSpinBox(
        const std::string& labelText,
        int min,
        int max,
        int defaultValue
) : VSpinBox( nullptr,
              labelText,
              min,
              max,
              defaultValue
    )
{}

void VSpinBox::SetValue( int value ) {
     if ( value != _value &&
          value <= _max   &&
          value >= _min   )
    {
        _value = value;
        _spinBox->setValue( value );
    }
}

int VSpinBox::GetValue() const {
    return _value;
}

void VSpinBox::_validateAndEmit() {
    int newValue = _spinBox->value();

     if ( newValue != _value &&
          newValue <= _max   &&
          newValue >= _min   )
    {
        _value = newValue;
        emit valueChanged();
    }
}

void VSpinBox::SetRange( int min, int max ) {
    if ( max < min ) VAssert( max > min );

    _min = min;
    _max = max;

    _spinBox->setMinimum( min );
    _spinBox->setMaximum( max );
}*/

/*
VDoubleSpinBox::VDoubleSpinBox(
        QWidget *parent,
        const std::string& labelText,
        double defaultValue
    ) :
    VLabel(parent, labelText),
    _value( defaultValue )
{
    _spinBox = new QDoubleSpinBox( this );
    _layout->addWidget( _spinBox );

    SetValue( defaultValue );

    connect( _spinBox, SIGNAL( editingFinished() ),
        this, SLOT( _changed() ) );
}

void VDoubleSpinBox::_changed() {
    double newValue = _spinBox->value();
    if ( newValue != _value ) {
        _value = newValue;
        emit valueChanged();
    }
}

void VDoubleSpinBox::SetMaximum( double maximum ) {
    _spinBox->setMaximum( maximum );
}

void VDoubleSpinBox::SetMinimum( double minimum ) {
    _spinBox->setMinimum( minimum );
}

void VDoubleSpinBox::SetValue( double value ) {
    _spinBox->setValue( value );
}

void VDoubleSpinBox::SetDecimals( int decimals ) {
    _spinBox->setDecimals( decimals );
}

double VDoubleSpinBox::GetValue() const {
    return _value;
}

*/

//
// ====================================
//
/*VRange::VRange(
    QWidget* parent,
    double min,
    double max,
    const std::string& minLabel,
    const std::string& maxLabel
) :
    VaporWidget( parent )
{
    _layout    = new QVBoxLayout(this);
    _layout->setContentsMargins( 0, 0, 0, 0 );

    _minSlider = new VSlider( this, minLabel, min, max, min );
    _maxSlider = new VSlider( this, maxLabel, min, max, max );
    connect( _minSlider, SIGNAL( valueChanged() ), this, SLOT( _respondMinSlider() ) );
    connect( _maxSlider, SIGNAL( valueChanged() ), this, SLOT( _respondMaxSlider() ) );

    _layout->addWidget( _minSlider );
    _layout->addWidget( _maxSlider );
}

VRange::VRange(
    double min,
    double max,
    const std::string& minLabel,
    const std::string& maxLabel
) : VRange(
        nullptr,
        min,
        max,
        minLabel,
        maxLabel
    )
{}

VRange::~VRange() {}

void
VRange::SetSelection( double min, double max ) {
    VAssert( min <= max );
    _minSlider->SetValue( min );
    _maxSlider->SetValue( max );
}

void
VRange::GetSelection( double& min, double& max ) const {
    min = _minSlider->GetValue();
    max = _maxSlider->GetValue();
}

void
VRange::SetExtents( double extentsLow, double extentsHigh )
{
    VAssert( extentsHigh >= extentsLow );
    _minSlider->SetExtents( extentsLow, extentsHigh );
    _maxSlider->SetExtents( extentsLow, extentsHigh );
}

void
VRange::_adjustMaxToMin()
{
    double low = _minSlider->GetValue();
    double high = _maxSlider->GetValue();
    if( low > high )
        _maxSlider->SetValue( low );
}

void
VRange::_adjustMinToMax()
{
    double low = _minSlider->GetValue();
    double high = _maxSlider->GetValue();
    if( high < low )
        _minSlider->SetValue( high );
}

void
VRange::_respondMinSlider()
{
    _adjustMaxToMin();
    emit valueChanged();
}

void
VRange::_respondMaxSlider()
{
    _adjustMinToMax();
    emit valueChanged();
}*/

//
// ====================================
//
/*VSlider::VSlider(
    QWidget* parent,
    const std::string& label,
    double min,
    double max,
    double value )
:
    VLabel( parent, label ),
    _min( min ),
    _max( max ),
    _value( value )
{
    VAssert( _max > _min );
    _value = (_min + _max) / 2.0f;

    _qslider = new QSlider( this );
    _qslider->setOrientation( Qt::Horizontal );
    // QSlider will always have its range in integers from 0 to 100.
    _qslider->setMinimum( 0 );
    _qslider->setMaximum( 100 );
    connect( _qslider, SIGNAL( sliderReleased() ), this, SLOT( _respondQSliderReleased() ) );
    connect( _qslider, SIGNAL( sliderMoved(int) ), this, SLOT( _respondQSliderMoved(int) ) );
    _layout->addWidget( _qslider );

    _qedit = new QLineEdit( this );
    connect( _qedit, SIGNAL( editingFinished() ), this, SLOT( _respondQLineEdit() ) );
    _layout->addWidget( _qedit );

    // update widget display
    double percent = (_value - _min) / (_max - _min) * 100.0f;
    _qslider->setValue( std::lround( percent ) );
    _qedit->setText( QString::number( _value, 'f', 3 ) );
}

VSlider::VSlider(
    const std::string& label,
    double min,
    double max,
    double value )
: VSlider(
        nullptr,
        label,
        min,
        max,
        value
) {}

VSlider::~VSlider() {}

void
VSlider::SetExtents( double min, double max )
{
    VAssert( min <= max );
    _min = min;
    _max = max;

    // keep the old _value if it's still within the range.
    //   Otherwise, re-assign the middle point to _value
    if( _value < min ||  _value > max )
    {
        _value   = (min + max) / 2.0f;
        _qedit->setText( QString::number( _value, 'f', 3 ) );
    }

    // update the slider position based on new range.
    double percent = (_value - _min) / (_max - _min) * 100.0f;
    _qslider->setValue( std::lround( percent ) );
}

void
VSlider::SetValue( double val )
{
    // Only respond if val is within range
    if( val >= _min && val <= _max )
    {
        _value   = val;
        double percent = (_value - _min) / (_max - _min) * 100.0f;
        _qslider->setValue( std::lround( percent ) );
        _qedit->setText( QString::number( _value, 'f', 3 ) );
    }
}

double
VSlider::GetValue() const
{
    return _value;
}

void
VSlider::_respondQSliderReleased()
{
    // QSlider is always giving a valid value, so no need to validate range
    int newvalI    = _qslider->value();
    double percent  = (double)newvalI / 100.0f;
    _value = _min + percent * (_max - _min);
    _qedit->setText( QString::number( _value, 'f', 3 ) );

    emit valueChanged();
}

void
VSlider::_respondQSliderMoved( int newPos )
{
    // QSlider is always at a valid position, so no need to validate range
    double percent   = (double)newPos / 100.0f;
    double tmpVal    = _min + percent * (_max - _min);
    _qedit->setText( QString::number( tmpVal, 'f', 3 ) );
    emit valueChanged();
}

void
VSlider::_respondQLineEdit()
{
    std::string newtext = _qedit->text().toStdString();
    double   newval;

    try
    {
        newval = std::stof( newtext );
    }
    catch ( const std::invalid_argument& e )
    {
        _qedit->setText( QString::number( _value, 'f', 3 ) );
        return;
    }

    // Now validate the input is within range
    if( newval < _min || newval > _max )
    {
        _qedit->setText( QString::number( _value, 'f', 3 ) );
        return;
    }

    // Now update _value, _qslider, and emit signal
    _value   = newval;
    double percent = (_value - _min) / (_max - _min) * 100.0f;
    _qslider->setValue( std::lround( percent ) );

    emit valueChanged();
}*/

//
// ====================================
//

/*VGeometry::VGeometry(
        QWidget* parent,
        const std::vector<double>& range,
        const std::vector<std::string>& label
) : VaporWidget( parent )
{
    _dim = range.size() / 2;
    VAssert( _dim == 2 || _dim == 3 );

    for( int i = 0; i < _dim; i++ )
        VAssert( range[ i ] < range[ i+3 ] );

    //QWidget* pageWidget = new QWidget();
    _layout = new QVBoxLayout( this );
    _layout->setContentsMargins( 0, 0, 0, 0 );
//    setLayout( layout );

    _xrange = new VRange( this, range[ XMIN ], range[ XMAX ], label[ XMIN ], label[ XMAX ] );
    _yrange = new VRange( this, range[ YMIN ], range[ YMAX ], label[ YMIN ], label[ YMAX ] );
    if( _dim == 3 )
        _zrange = new VRange( this, range[ ZMIN ], range[ ZMAX ], label[ ZMIN ], label[ ZMAX ] );
    else    // Create anyway. Will be hidden though.
    {
        _zrange = new VRange( this, 0.0f, 100.0f, "ZMin", "ZMax" );
        _zrange->hide();
    }

    connect( _xrange, SIGNAL( valueChanged() ), this, SLOT( _respondChanges() ) );
    connect( _yrange, SIGNAL( valueChanged() ), this, SLOT( _respondChanges() ) );
    connect( _zrange, SIGNAL( valueChanged() ), this, SLOT( _respondChanges() ) );

    //_layout = new QVBoxLayout(this);
    _layout->addWidget( _xrange );
    _layout->addWidget( _yrange );
    _layout->addWidget( _zrange );
    //addTab( pageWidget, QString::fromStdString(label) );
}

VGeometry::VGeometry(
        const std::vector<double>& range,
        const std::vector<std::string>& label
) : VGeometry( nullptr, range, label ) {}

VGeometry::~VGeometry() {}

void
VGeometry::SetRange( const std::vector<double>& range )
{
    VAssert( range.size() == _dim * 2 );
    _dim = range.size() / 2;
    VAssert( _dim == 2 || _dim == 3 );

    for( int i = 0; i < _dim; i++ )
        VAssert( range[ i ] < range[ i+3 ] );

    // Adjust the appearance if necessary
    if( _dim == 3 )
        _zrange->show();
    else
        _zrange->hide();

    _xrange->SetExtents( range[ XMIN ], range[ XMAX ] );
    _yrange->SetExtents( range[ YMIN ], range[ YMAX ] );
    if( _dim == 3 )
        _zrange->SetExtents( range[ ZMIN ], range[ ZMAX ] );
}

void
VGeometry::SetGeometry( const std::vector<double>& vals )
{
    if ( vals.size() != _dim * 2 )
        return;

    for( int i = 0; i < _dim; i++ )
        VAssert( vals[ i ] <= vals[ i+3 ] );

    // VRange widgets will only respond to values within their ranges
    _xrange->SetSelection( vals[ XMIN ], vals[ XMAX ] );
    _yrange->SetSelection( vals[ YMIN ], vals[ YMAX ] );
    if( _dim == 3 )
        _zrange->SetSelection( vals[ ZMIN ], vals[ ZMAX ] );
}

void
VGeometry::GetGeometry( std::vector<double>& vals ) const
{
    vals.resize( _dim * 2, 0.0f );
    _xrange->GetSelection( vals[ XMIN ], vals[ XMAX ] );
    _yrange->GetSelection( vals[ YMIN ], vals[ YMAX ] );
    if( _dim == 3 )
        _zrange->GetSelection( vals[ ZMIN ], vals[ ZMAX ] );
}

void
VGeometry::_respondChanges()
{
    emit valueChanged();
}*/

/*

//
// ====================================
//
VLineEdit::VLineEdit(
        QWidget *parent,
        const std::string& labelText,
        const std::string& editText
    ) :
    VLine(parent, labelText)
{
    _text = editText;

    _edit = new QLineEdit( this );
    _layout->addWidget( _edit );

    SetEditText( QString::fromStdString( editText ) );

    connect( _edit, SIGNAL( editingFinished() ),
        this, SLOT( _relaySignal() ) );
}

VLineEdit::~VLineEdit() { }

void VLineEdit::SetEditText( const std::string& text )
{
    SetEditText(QString::fromStdString( text ) );
}

void VLineEdit::SetEditText( const QString& text )
{
    _edit->setText( text );
    _text = _edit->text().toStdString();
}

std::string VLineEdit::GetEditText() const {
    return _text;
}

void VLineEdit::_relaySignal() {
    QString text = _edit->text();
    _edit->setText( text );
    _text = text.toStdString();

    emit _editingFinished();
}

//
// ====================================
//

VPushButton::VPushButton(
        QWidget *parent,
        const std::string& labelText,
        const std::string& buttonText
    ) :
    VLine(parent, labelText)
{
    _button = new QPushButton( this );
    _layout->addWidget( _button );

    SetButtonText( QString::fromStdString( buttonText ) );

    connect( _button, SIGNAL( pressed() ),
        this, SLOT( _buttonPressed() ) );
}

void VPushButton::SetButtonText( const std::string& text )
{
    SetButtonText(QString::fromStdString( text ) );
}

void VPushButton::SetButtonText( const QString& text )
{
    _button->setText( text );
}

void VPushButton::_buttonPressed() {
    emit _pressed();
}

VComboBox::VComboBox(
        QWidget *parent,
        const std::string& labelText
    ) :
    VLine(parent, labelText)
{
    _combo = new QComboBox(this);
    _layout->addWidget( _combo );

    connect( _combo, SIGNAL( currentIndexChanged(int) ),
        this, SLOT( _userIndexChanged(int) ) );
}

void VComboBox::_userIndexChanged(int index) {
    emit _indexChanged(index);
}

int VComboBox::GetNumOfItems() const
{
    return _combo->count();
}

int VComboBox::GetCurrentIndex() const {
    return _combo->currentIndex();
}

std::string VComboBox::GetCurrentText() const {
    return _combo->currentText().toStdString();
}

void VComboBox::AddOption( const std::string& option, int index) {
    _combo->insertItem( index, QString::fromStdString(option) );
}

void VComboBox::RemoveOption( int index=0 ) {
    _combo->removeItem( index ) ;
}

void VComboBox::SetIndex( int index ) {
    _combo->setCurrentIndex( index );
}

VCheckBox::VCheckBox(
        QWidget *parent,
        const std::string& labelText
    ) :
    VLine(parent, labelText)
{
    _checkbox = new QCheckBox( "", this );
    _layout->addWidget( _checkbox );

    _layout->setContentsMargins( 10, 0, 16, 0);

    connect( _checkbox, SIGNAL( stateChanged(int) ),
        this, SLOT( _userClickedCheckbox() ) );
}

bool VCheckBox::GetCheckState() const {
    if ( _checkbox->checkState() == Qt::Checked)
        return true;
    else
        return false;
}

void VCheckBox::SetCheckState( bool checkState ) {
    if ( checkState )
        _checkbox->setCheckState( Qt::Checked );
    else
        _checkbox->setCheckState( Qt::Unchecked );
}

void VCheckBox::_userClickedCheckbox() {
    emit _checkboxClicked();
}

VFileSelector::VFileSelector(
        QWidget *parent,
        const std::string& labelText,
        const std::string& buttonText,
        const std::string& filePath,
        QFileDialog::FileMode fileMode
    ) :
    VPushButton(parent, labelText, buttonText),
    _filePath( filePath )
{
    _lineEdit = new QLineEdit(this);
    _layout->addWidget( _lineEdit );

    QString defaultPath = QString::fromStdString( GetPath() );
    if ( _filePath.empty() )
        defaultPath = QDir::homePath();

    _fileDialog = new QFileDialog(
        this,
        QString::fromStdString( labelText ),
        defaultPath
    );

    _fileMode = fileMode;
    _fileDialog->setFileMode( _fileMode );

    _lineEdit->setText( QString::fromStdString( filePath ) );

    connect( _button, SIGNAL( pressed() ),
        this, SLOT( _openFileDialog() ) );
    connect( _lineEdit, SIGNAL( returnPressed() ),
        this, SLOT( _setPathFromLineEdit() ) );
}

std::string VFileSelector::GetPath() const {
    return _filePath;
}

void VFileSelector::SetPath( const QString& path) {
    SetPath( path.toStdString() );
}

void VFileSelector::SetPath( const std::string& path ) {
    if ( path.empty() )
        return;

    if ( !_isFileOperable( path ) ) {
        MSG_ERR(
            FileOperationChecker::GetLastErrorMessage().toStdString()
        );
        _lineEdit->setText( QString::fromStdString( _filePath ) );
        return;
    }
    _filePath = path;
    _lineEdit->setText( QString::fromStdString(path) );
}

void VFileSelector::SetFileFilter( const QString& filter ) {
    _fileDialog->setNameFilter( filter );
}

void VFileSelector::SetFileFilter( const std::string& filter) {
    _fileDialog->setNameFilter( QString::fromStdString(filter) );
}

void VFileSelector::_openFileDialog() {

    if (_fileDialog->exec() != QDialog::Accepted) {
        _button->setDown(false);
        return;
    }

    QStringList files = _fileDialog->selectedFiles();
    if (files.size() != 1) {
        _button->setDown(false);
        return;
    }

    QString filePath = files[0];

    SetPath( filePath.toStdString() );
    _button->setDown(false);

    emit _pathChanged();
}

void VFileSelector::_setPathFromLineEdit() {
    QString filePath = _lineEdit->text();
    SetPath( filePath.toStdString() );
    emit _pathChanged();
}

VFileReader::VFileReader(
        QWidget *parent,
        const std::string& labelText,
        const std::string& buttonText,
        const std::string& filePath
    ) :
    VFileSelector(
        parent,
        labelText,
        buttonText,
        filePath,
        QFileDialog::FileMode::ExistingFile
    )
{}

bool VFileReader::_isFileOperable( const std::string& filePath ) const {
    bool operable = false;
    if ( _fileMode == QFileDialog::FileMode::ExistingFile ) {
        operable = FileOperationChecker::FileGoodToRead(
            QString::fromStdString( filePath ) );
    }
    if ( _fileMode == QFileDialog::FileMode::Directory ) {
        operable = FileOperationChecker::DirectoryGoodToRead(
            QString::fromStdString( filePath ) );
    }

    return operable;
}

VFileWriter::VFileWriter(
        QWidget *parent,
        const std::string& labelText,
        const std::string& buttonText,
        const std::string& filePath
    ) :
    VFileSelector(
        parent,
        labelText,
        buttonText,
        filePath
    )
{
    QFileDialog::AcceptMode acceptMode = QFileDialog::AcceptSave;
    _fileDialog->setAcceptMode( acceptMode );
    _fileMode = QFileDialog::AnyFile;
    _fileDialog->setFileMode( _fileMode );
}

bool VFileWriter::_isFileOperable( const std::string& filePath ) const {
    bool operable = false;
    QString qFilePath = QString::fromStdString( filePath );
    operable = FileOperationChecker::FileGoodToWrite( qFilePath );
    return operable;
}

VTabWidget::VTabWidget(
    QWidget* parent,
    const std::string& firstTabName
) : QTabWidget( parent )
{
    AddTab( firstTabName );
}

void VTabWidget::AddTab(
    const std::string& tabName
) {
    QWidget* container = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->setContentsMargins( 0, 0, 0, 0 );
    container->setLayout(layout);

    addTab( container, QString::fromStdString(tabName) );
}

void VTabWidget::DeleteTab(
    int index
) {
    removeTab( index );
}

void VTabWidget::AddWidget(
    QWidget* inputWidget,
    int index
) {
    QWidget* target = widget(index);
    target->layout()->addWidget( inputWidget );
}*/
