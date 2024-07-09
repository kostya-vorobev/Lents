#include "postclass.h"

PostClass::PostClass(int postID, const QString& author, const QString& postText, int countLike, int countDislike)
    : postID(postID), author(author), postText(postText), countLike(countLike), countDislike(countDislike) {}

// Геттеры
int PostClass::getID() { return postID; }
QString PostClass::getAuthor()  { return author; }
QString PostClass::getPostText()  { return postText; }
int PostClass::getCountLike()  { return countLike; }
int PostClass::getCountDislike()  { return countDislike; }

// Сеттеры
void PostClass::setID(int postID) { this->postID = postID; }
void PostClass::setAuthor(const QString& author) { this->author = author; }
void PostClass::setPostText(const QString& postText) { this->postText = postText; }
void PostClass::setCountLike(int countLike) { this->countLike = countLike; }
void PostClass::setCountDislike(int countDislike) { this->countDislike = countDislike; }

// Методы для увеличения/уменьшения количества лайков и дизлайков
void PostClass::incrementLikes() { ++countLike; }
void PostClass::decrementLikes() { if (countLike > 0) --countLike; }
void PostClass::incrementDislikes() { ++countDislike; }
void PostClass::decrementDislikes() { if (countDislike > 0) --countDislike; }
