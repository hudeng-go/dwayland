/********************************************************************
Copyright 2014  Martin Gräßlin <mgraesslin@kde.org>

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) version 3, or any
later version accepted by the membership of KDE e.V. (or its
successor approved by the membership of KDE e.V.), which shall
act as a proxy defined in Section 6 of version 3 of the license.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*********************************************************************/
#ifndef WAYLAND_DATASOURCE_H
#define WAYLAND_DATASOURCE_H

#include "buffer.h"

#include <QObject>

#include <KWayland/Client/kwaylandclient_export.h>

struct wl_data_source;
class QMimeType;

namespace KWayland
{
namespace Client
{


/**
 * @short Wrapper for the wl_data_source interface.
 *
 * This class is a convenient wrapper for the wl_data_source interface.
 * To create a DataSource call DataDeviceManager::createDataSource.
 *
 * @see DataDeviceManager
 **/
class KWAYLANDCLIENT_EXPORT DataSource : public QObject
{
    Q_OBJECT
public:
    explicit DataSource(QObject *parent = nullptr);
    virtual ~DataSource();

    /**
     * Setup this DataSource to manage the @p dataSource.
     * When using DataDeviceManager::createDataSource there is no need to call this
     * method.
     **/
    void setup(wl_data_source *dataSource);
    /**
     * Releases the wl_data_source interface.
     * After the interface has been released the DataSource instance is no
     * longer valid and can be setup with another wl_data_source interface.
     **/
    void release();
    /**
     * Destroys the data hold by this DataSource.
     * This method is supposed to be used when the connection to the Wayland
     * server goes away. If the connection is not valid any more, it's not
     * possible to call release any more as that calls into the Wayland
     * connection and the call would fail. This method cleans up the data, so
     * that the instance can be deleted or setup to a new wl_data_source interface
     * once there is a new connection available.
     *
     * It is suggested to connect this method to ConnectionThread::connectionDied:
     * @code
     * connect(connection, &ConnectionThread::connectionDied, source, &DataSource::destroyed);
     * @endcode
     *
     * @see release
     **/
    void destroy();
    /**
     * @returns @c true if managing a wl_data_source.
     **/
    bool isValid() const;

    void offer(const QString &mimeType);
    void offer(const QMimeType &mimeType);

    operator wl_data_source*();
    operator wl_data_source*() const;

Q_SIGNALS:
    /**
     * Emitted when a target accepts pointer_focus or motion events. If
     * a target does not accept any of the offered types, @p mimeType is empty.
     **/
    void targetAccepts(const QString &mimeType);
    /**
     * Request for data from the client. Send the data as the
     * specified @p mimeType over the passed file descriptor @p fd, then close
     * it.
     **/
    void sendDataRequested(const QString &mimeType, qint32 fd);
    /**
     * This DataSource has been replaced by another DataSource.
     * The client should clean up and destroy this DataSource.
     **/
    void cancelled();

private:
    class Private;
    QScopedPointer<Private> d;
};

}
}

#endif