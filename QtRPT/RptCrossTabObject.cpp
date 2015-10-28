/*
Name: QtRpt
Version: 1.5.5
Web-site: http://www.qtrpt.tk
Programmer: Aleksey Osipov
E-mail: aliks-os@ukr.net
Web-site: http://www.aliks-os.tk

Copyright 2012-2015 Aleksey Osipov

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "RptCrossTabObject.h"

RptCrossTabObject::RptCrossTabObject() {
    colHeaderVisible = true;
    rowHeaderVisible = true;
    name = "RptCrossTabObject_DEMO";
    rect.setX(0);
    rect.setY(0);
    rect.setWidth(500);
    rect.setHeight(500);

    qRegisterMetaType<RptCrossTabObject>( "RptCrossTabObject" );
}

int RptCrossTabObject::colCount() const {
    return m_colHeader.size();
}

int RptCrossTabObject::rowCount() const {
    return m_rowHeader.size();
}

void RptCrossTabObject::addCol(QString colName) {
    m_colHeader << colName;
}

void RptCrossTabObject::addRow(QString rowName) {
    m_rowHeader << rowName;
}

QString RptCrossTabObject::getColName(int col) const {
    return m_colHeader[col];
}

QString RptCrossTabObject::getRowName(int row) const {
    return m_rowHeader[row];
}

void RptCrossTabObject::initMatrix() {
    valuesArray.resize(m_rowHeader.size());  //Set row count

    QMutableVectorIterator<QVectorVariant> iRows(valuesArray);
    while (iRows.hasNext())
        (iRows.next()).resize(m_colHeader.size());
}

QVariant RptCrossTabObject::getMatrixValue(int col,int row) const {
    QVectorVariant rowValue = valuesArray[row];
    return rowValue[col];
}

void RptCrossTabObject::setMatrixValue(int col,int row, QVariant value) {
    QVectorVariant rowValue = valuesArray[row];
    rowValue[col] = value;
}

void RptCrossTabObject::paintCrossTab(QPainter *painter) {
    float fieldWidth = rect.width();
    float fieldheight = rect.height();
    if (colCount() == 0) return;
    if (rowCount() == 0) return;
    if (isRowHeaderVisible()) fieldWidth = rect.width()/colCount()+1;
    else fieldWidth = rect.width()/colCount();
    if (isColHeaderVisible()) fieldheight = rect.height()/rowCount()+1;
    else fieldheight = rect.height()/rowCount();

    for (int row=0; row < rowCount(); row++) {
        if (isRowHeaderVisible()) {
            //Make a rowHeader
            RptFieldObject *h1 = new RptFieldObject();
            h1->name = QString("rh%1").arg(row);
            h1->fieldType = Text;
            h1->rect.setTop(rect.top() + fieldheight + fieldheight*row);
            h1->rect.setLeft(rect.left());
            h1->rect.setHeight(fieldheight);
            h1->rect.setWidth(fieldWidth);
            h1->value = m_rowHeader[row];
            h1->font.setBold(true);
            h1->setDefaultBackgroundColor(Qt::lightGray); //Set default background color
            h1->aligment = Qt::AlignCenter;
            addField(h1);  //Append field
        }
        for (int col=0; col < colCount(); col++) {
            if (row == 0 && isColHeaderVisible()) {
                //Make a colHeader
                RptFieldObject *h1 = new RptFieldObject();
                h1->name = QString("ch%1").arg(col);
                h1->fieldType = Text;
                h1->rect.setTop(rect.top());
                h1->rect.setLeft(rect.left() + fieldWidth + fieldWidth*col);
                h1->rect.setHeight(fieldheight);
                h1->rect.setWidth(fieldWidth);
                h1->value = m_colHeader[col];
                h1->font.setBold(true);
                h1->setDefaultBackgroundColor(Qt::lightGray); //Set default background color
                h1->aligment = Qt::AlignCenter;
                addField(h1);  //Append field
            }

            //Fill values
            RptFieldObject *h1 = new RptFieldObject();
            h1->name = QString("f%1%2").arg(col).arg(row);
            h1->fieldType = Text;
            h1->rect.setTop(rect.top());  //!!!
            h1->rect.setLeft(rect.left() + fieldWidth + fieldWidth*col);  //!!!
            h1->rect.setHeight(fieldheight);
            h1->rect.setWidth(fieldWidth);
            h1->value = m_colHeader[col];
            h1->font.setBold(true);
            h1->setDefaultBackgroundColor(Qt::lightGray); //Set default background color
            h1->aligment = Qt::AlignCenter;
            addField(h1);  //Append field
        }
    }
}

/*!
 \fn void RptCrossTabObject::addField(RptFieldObject *field)
    Add \a field to current RptCrossTabObject object.

    \sa RptFieldObject
*/
void RptCrossTabObject::addField(RptFieldObject *field) {
    //field->parentBand = this;
    fieldList.append(field);
}

/*!
  Destroys the object, deleting all its child objects.
 */
RptCrossTabObject::~RptCrossTabObject() {
    for (int i=0; i<fieldList.size(); i++)
        delete fieldList.at(i);
    fieldList.clear();
}

QDebug operator<<(QDebug dbg, const RptCrossTabObject &obj) {
    dbg << obj.name << "\n";

    if (obj.isColHeaderVisible()) {
        dbg << "\t";
        for(int col=0; col<obj.colCount(); col++) {
            dbg << "|" << obj.getColName(col) << "\t";
        }
        dbg << "\n";
    }

    for(int row=0; row<obj.rowCount(); row++) {
        if (obj.isRowHeaderVisible()) {
            dbg << obj.getRowName(row);
            dbg << "\t";
        }

        for(int col=0; col<obj.colCount(); col++) {
            dbg << "|" << obj.getMatrixValue(col,row) << "\t";
        }
        dbg << "\n";
    }
    return dbg;
}

QDebug operator<<(QDebug dbg, const RptCrossTabObject *obj) {
    return dbg << *obj;
}