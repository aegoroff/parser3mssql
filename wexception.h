/*!
 * \brief   The file contains wide char exception interface
 * \author  \verbatim
            Created by: Alexander Egorov
            \endverbatim
 * \date    \verbatim
            Creation date: 2010-04-11
            \endverbatim
 * Copyright 2008-2012 Alexander Egorov <egoroff@gmail.com> (http://www.egoroff.spb.ru)
 */

#pragma once

/**
 * \brief Exception that just wraps up wide chars message string.
 */
class wexception : public std::exception {
    public:
        /**
         * \brief Creates new instance with empty message
         */
        wexception();

        /**
         * \brief Creates new instance using message specified.
         * @param pMessage Pointer to message string
         */
        explicit wexception(__in const std::wstring* pMessage);

        /**
         * \brief Creates new instance using string stream specified. This class is responsible for deleting passed pointer
         * @param pMessageStream Pointer to stream that constructs message.
         */
        explicit wexception(__in const std::wostringstream* pMessageStream);

        /**
         * \brief Creates new instance using message specified.
         * @param pMessage Pointer to message.
         */
        explicit wexception(__in const wchar_t* pMessage);

        // we needn't make it virtual because base
        // class constructor is virtual so this one inherit it.
        ~wexception();

        /**
         * \brief Gets exception message
         */
        const std::wstring* GetMessage() const;

        /**
         * \brief Creates new copy of an instance
         */
        wexception(const wexception& rException);

        /**
         * \brief Replaces current instance by the instance specified.
         */
        wexception& operator =(const wexception& rException);
    private:
        const std::wstring* pMessage_;
        const std::wostringstream* pMessageStream_;
};

