#include "chatclass.h"

ChatClass::ChatClass(int id, int user1_id, QString user1_name, int user2_id, QString user2_name, QString message, QDateTime sent_at)
    : id(id), user1_id(user1_id), user1_name(user1_name), user2_id(user2_id), user2_name(user2_name), message(message), sent_at(sent_at){}

// Геттеры и сеттеры для user1_id
int ChatClass::getId()  { return id;}
void ChatClass::setId(int id) { this->id = id;}

// Геттеры и сеттеры для user1_id
int ChatClass::getUser1Id()  { return user1_id;}
void ChatClass::setUser1Id(int user1_id) { this->user1_id = user1_id;}

// Геттер и сеттер для user1_name
QString ChatClass::getUser1Name()  {return user1_name;}

void ChatClass::setUser1Name(QString user1_name) {this->user1_name = user1_name;}

// Геттер и сеттер для user2_id
int ChatClass::getUser2Id()  {return user2_id;}

void ChatClass::setUser2Id(int user2_id) {this->user2_id = user2_id;}

// Геттер и сеттер для user2_name
QString ChatClass::getUser2Name()  {return user2_name;}

void ChatClass::setUser2Name(QString user2_name) {this->user2_name = user2_name;}

// Геттер и сеттер для message
QString ChatClass::getMessage()  {return message;}

void ChatClass::setMessage(QString message) {this->message = message;}

// Геттер и сеттер для sent_at
QDateTime ChatClass::getSentAt()  {return sent_at;}

void ChatClass::setSentAt(QDateTime sent_at) {this->sent_at = sent_at;}

