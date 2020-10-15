/*
 * Copyright (C) 2015 ~ 2018 Wuhan Deepin Technology Co., Ltd.
 *
 * Author:     Iceyer <me@iceyer.net>
 *
 * Maintainer: Iceyer <me@iceyer.net>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
#include <QObject>
#include <QString>

#include "devicemonitor.h"

namespace Utils
{

void initResource();
void loadTranslate();
void ClearTargetDev(const QString &targetPath);
bool CheckInstallDisk(const QString &targetDev);

QMap<QString, DeviceInfo> CommandDfParse();
QMap<QString, DeviceInfo> CommandLsblkParse();
QList<DeviceInfo>  ListUsbDrives();

QString UsbShowText(const QString &targetdev);
}
