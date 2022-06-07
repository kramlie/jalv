/*
  Copyright 2007-2022 David Robillard <d@drobilla.net>

  Permission to use, copy, modify, and/or distribute this software for any
  purpose with or without fee is hereby granted, provided that the above
  copyright notice and this permission notice appear in all copies.

  THIS SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include "jalv_internal.h"

#include "lilv/lilv.h"
#include "suil/suil.h"
#include "zix/sem.h"

#include <QtGlobal>

#include <QAction>
#include <QApplication>
#include <QDial>
#include <QFontMetrics>
#include <QGroupBox>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QKeySequence>
#include <QLabel>
#include <QLayout>
#include <QLayoutItem>
#include <QList>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QObject>
#include <QPoint>
#include <QRect>
#include <QScreen>
#include <QScrollArea>
#include <QSize>
#include <QSizePolicy>
#include <QString>
#include <QStyle>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>
#include <QtCore>

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <map>
#include <vector>

class PresetAction : public QAction
{
  Q_OBJECT // NOLINT

public:
  PresetAction(QObject* parent, Jalv* jalv, LilvNode* preset)
    : QAction(parent)
    , _jalv(jalv)
    , _preset(preset)
  {
    connect(this, SIGNAL(triggered()), this, SLOT(presetChosen()));
  }

  Q_SLOT void presetChosen() { jalv_apply_preset(_jalv, _preset); }

private:
  Jalv*     _jalv;
  LilvNode* _preset;
};

struct PortContainer {
  Jalv*        jalv;
  struct Port* port;
};

class Control : public QGroupBox
{
  Q_OBJECT // NOLINT

public:
  explicit Control(PortContainer portContainer, QWidget* parent);

  Q_SLOT void dialChanged(int value);

  void setValue(float value);

private:
  void    setRange(float min, float max);
  QString getValueLabel(float value);
  float   getValue();
  int     stringWidth(const QString& str);

  QDial*            dial;
  const LilvPlugin* plugin;
  struct Port*      port;

  QLabel* label;
  QString name;
  int     steps;
  float   max;
  float   min;
  bool    isInteger;
  bool    isEnum;
  bool    isLogarithmic;

  std::vector<float>           scalePoints;
  std::map<float, const char*> scaleMap;
};
