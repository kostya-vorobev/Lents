#include "postclass.h"

PostClass::PostClass(const std::string& author, const std::string& postText, int countLike, int countDislike)
    : author(author), postText(postText), countLike(countLike), countDislike(countDislike) {}

// Геттеры
std::string PostClass::getAuthor()  { return author; }
std::string PostClass::getPostText()  { return postText; }
int PostClass::getCountLike()  { return countLike; }
int PostClass::getCountDislike()  { return countDislike; }

// Сеттеры
void PostClass::setAuthor(const std::string& author) { this->author = author; }
void PostClass::setPostText(const std::string& postText) { this->postText = postText; }
void PostClass::setCountLike(int countLike) { this->countLike = countLike; }
void PostClass::setCountDislike(int countDislike) { this->countDislike = countDislike; }

// Методы для увеличения/уменьшения количества лайков и дизлайков
void PostClass::incrementLikes() { ++countLike; }
void PostClass::decrementLikes() { if (countLike > 0) --countLike; }
void PostClass::incrementDislikes() { ++countDislike; }
void PostClass::decrementDislikes() { if (countDislike > 0) --countDislike; }
