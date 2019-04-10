#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QByteArray>
#include <QJsonObject>
#include <QObject>
#include <QString>

/*
 *@sender
 *  #ip
 *  #port
 *  #user
 *
 * @action  #major order
 *      +handshake   #tell its role to remote server
 *          from : QString  # role
 *
 *  @expect   #static information tha t must be written in reply
 *
 *  @request
 *  @reply
 *  @command    tell remote to do somthing
 *  @pull         get some data
 *  @push       send some data
 *
 */
#endif  //! PROTOCOL_H
