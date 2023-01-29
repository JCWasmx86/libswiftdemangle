//===--- MacroDeclaration.h - Swift Macro Declaration -----------*- C++ -*-===//
//
// This source file is part of the Swift.org open source project
//
// Copyright (c) 2014 - 2020 Apple Inc. and the Swift project authors
// Licensed under Apache License v2.0 with Runtime Library Exception
//
// See https://swift.org/LICENSE.txt for license information
// See https://swift.org/CONTRIBUTORS.txt for the list of Swift project authors
//
//===----------------------------------------------------------------------===//
//
// Data structures that configure the declaration of a macro.
//
//===----------------------------------------------------------------------===//

#ifndef SWIFT_AST_MACRO_DECLARATION_H
#define SWIFT_AST_MACRO_DECLARATION_H

#include "swift/AST/Identifier.h"

namespace swift {

/// Describes the syntax that is used for a macro, which affects its role.
enum class MacroSyntax: uint8_t {
  /// Freestanding macro syntax starting with an explicit '#' followed by the
  /// macro name and optional arguments.
  Freestanding,

  /// Attached macro syntax written as an attribute with a leading `@` followed
  /// by the macro name an optional arguments.
  Attached,
};

/// The context in which a macro can be used, which determines the syntax it
/// uses.
enum class MacroRole: uint32_t {
  /// An expression macro, referenced explicitly via "#stringify" or similar
  /// in the source code.
  Expression = 0x01,
  /// A freestanding declaration macro.
  Declaration = 0x02,
  /// An attached macro that declares accessors for a variable or subscript
  /// declaration.
  Accessor = 0x04,
  /// An attached macro that generates attributes for the
  /// members inside the declaration.
  MemberAttribute = 0x08,
  /// An attached macro that generates synthesized members
  /// inside the declaration.
  Member = 0x10,
};

/// The contexts in which a particular macro declaration can be used.
using MacroRoles = OptionSet<MacroRole>;

/// Retrieve the string form of the given macro role, as written on the
/// corresponding attribute.
StringRef getMacroRoleString(MacroRole role);

/// Whether a macro with the given set of macro contexts is freestanding, i.e.,
/// written in the source code with the `#` syntax.
bool isFreestandingMacro(MacroRoles contexts);

/// Whether a macro with the given set of macro contexts is attached, i.e.,
/// written in the source code as an attribute with the `@` syntax.
bool isAttachedMacro(MacroRoles contexts);

enum class MacroIntroducedDeclNameKind {
  Named,
  Overloaded,
  Prefixed,
  Suffixed,
  Arbitrary,
};

/// Whether a macro-introduced name of this kind requires an argument.
bool macroIntroducedNameRequiresArgument(MacroIntroducedDeclNameKind kind);

StringRef getMacroIntroducedDeclNameString(
    MacroIntroducedDeclNameKind kind);

class CustomAttr;

/// Perform lookup to determine whether the given custom attribute refers to
/// a macro declaration, and populate the \c macros vector with the lookup
/// results that are attached macros.
void findMacroForCustomAttr(CustomAttr *attr, DeclContext *dc,
                            llvm::TinyPtrVector<ValueDecl *> &macros);

class MacroIntroducedDeclName {
public:
  using Kind = MacroIntroducedDeclNameKind;

private:
  Kind kind;
  Identifier identifier;

public:
  MacroIntroducedDeclName(Kind kind, Identifier identifier = Identifier())
      : kind(kind), identifier(identifier) {};

  static MacroIntroducedDeclName getNamed(Identifier name) {
    return MacroIntroducedDeclName(Kind::Named, name);
  }

  static MacroIntroducedDeclName getOverloaded() {
    return MacroIntroducedDeclName(Kind::Overloaded);
  }

  static MacroIntroducedDeclName getPrefixed(Identifier prefix) {
    return MacroIntroducedDeclName(Kind::Prefixed, prefix);
  }

  static MacroIntroducedDeclName getSuffixed(Identifier suffix) {
    return MacroIntroducedDeclName(Kind::Suffixed, suffix);
  }

  static MacroIntroducedDeclName getArbitrary() {
    return MacroIntroducedDeclName(Kind::Arbitrary);
  }

  Kind getKind() const { return kind; }
  Identifier getIdentifier() const { return identifier; }
};

}

#endif // SWIFT_AST_MACRO_DECLARATION_H
