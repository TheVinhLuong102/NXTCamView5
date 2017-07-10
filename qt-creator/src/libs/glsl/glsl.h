/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
****************************************************************************/

#ifndef GLSL_H
#define GLSL_H

#include <qglobal.h>
#include <cstdlib>
#include <cstddef>

#if defined(GLSL_BUILD_LIB)
#  define GLSL_EXPORT Q_DECL_EXPORT
#elif defined(GLSL_BUILD_STATIC_LIB)
#  define GLSL_EXPORT
#else
#  define GLSL_EXPORT Q_DECL_IMPORT
#endif

namespace GLSL {
class Engine;
class Lexer;
class Parser;
class MemoryPool;

// types
class Type;
class UndefinedType;
class VoidType;
class ScalarType;
class BoolType;
class IntType;
class UIntType;
class FloatType;
class DoubleType;
class IndexType;
class VectorType;
class MatrixType;
class ArrayType;
class SamplerType;

// symbols
class Symbol;
class Scope;
class Struct;
class Function;
class Argument;
class Block;
class Variable;
class OverloadSet;
class Namespace;

class AST;
class TranslationUnitAST;
template <typename T> class List;
}

#endif // GLSL_H
