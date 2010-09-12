/*
 *  ast.cpp
 *  Untitled
 *
 *  Created by Daniel on 16/01/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ast.h"
#include "symboltable.h"

using namespace Basic;

void Basic::LiteralExpression::execute (void) {
    ; // m_result is set at initialisation
}

void Basic::IdentifierExpression::execute (void) {
    
}

void Basic::UnaryExpression::execute (void) {
    this->term->execute();
    Variant intermediate = term->getResult();
    if (this->op == TkNOT) {
        m_result.setBoolValue(!intermediate.getBoolValue());
    }
    else if (this->op == TkMINUS) {
        switch(intermediate.getType()) {
            case Basic::Variant::VaINT:
                m_result.setIntValue(0 - intermediate.getIntValue());
                break;
            case Basic::Variant::VaDOUBLE:
                m_result.setDoubleValue(0.0 - intermediate.getDoubleValue());
                break;
            default:
                fprintf(stderr, "warning: attempt to negate non-numeric value at line %i, column %i\n", m_line, m_column);
                m_result = intermediate;
        }
    }
    else {
        fprintf(stderr, "debug: invalid operator `%s' in UnaryExpression object\n", Tokeniser::tokenDescriptions[this->op]);
    }
}

void Basic::MultiplicativeExpression::execute (void) {
    first_term->execute();
    Variant intermediate = first_term->getResult();
    
    for (std::list<OperatorTerm>::const_iterator term = other_terms.begin(); term != other_terms.end(); term++) {
        term->term->execute();
        Variant t = term->term->getResult();
        
        switch (term->op) {
            case TkMULTIPLY:    intermediate *= t; break;
            case TkDIVIDE:      intermediate /= t; break;
            default:
                fprintf(stderr, "debug: invalid operator `%s' in MultiplicativeExpression object\n", Tokeniser::tokenDescriptions[term->op]);
        }
    }
    
    m_result = intermediate;
}

void Basic::AdditiveExpression::execute (void) {
    first_term->execute();
    Variant intermediate = first_term->getResult();
    
    for (std::list<OperatorTerm>::const_iterator term = other_terms.begin(); term != other_terms.end(); term++) {
        term->term->execute();
        Variant t = term->term->getResult();
        
        switch (term->op) {
            case TkPLUS:    intermediate += t; break;
            case TkMINUS:   intermediate -= t; break;
            default:
                fprintf(stderr, "debug: invalid operator `%s' in AdditiveExpression object\n", Tokeniser::tokenDescriptions[term->op]);
        }
    }
    
    m_result = intermediate;
}

void Basic::ComparitiveExpression::execute (void) {
    bool intermediate = false;
    switch (this->cmp) {
        case TkEQUALS:      intermediate = (first == second); break;
        case TkNOTEQUALS:   intermediate = (first != second); break;
        case TkLT:          intermediate = (first <  second); break;
        case TkGT:          intermediate = (first >  second); break;
        case TkLTEQUALS:    intermediate = (first <= second); break;
        case TkGTEQUALS:    intermediate = (first >= second); break;
        default: 
            fprintf(stderr, "debug: invalid operator `%s' in ComparitiveExpression object\n", Tokeniser::tokenDescriptions[this->cmp]);
    }
    m_result.setBoolValue(intermediate);
}

void Basic::AndExpression::execute (void) {
    bool intermediate = true;
    for (std::list<Basic::Expression*>::const_iterator e = terms.begin(); e != terms.end(); e++) {
        (*e)->execute();
        intermediate = (intermediate and (*e)->getResult().getBoolValue());
    }
    m_result.setBoolValue(intermediate);
}

void Basic::OrExpression::execute (void) {
    bool intermediate = false;
    for (std::list<Basic::Expression*>::const_iterator e = terms.begin(); e != terms.end(); e++) {
        (*e)->execute();
        intermediate = (intermediate and (*e)->getResult().getBoolValue());
    }
    m_result.setBoolValue(intermediate);
}

void Basic::Block::execute (void) {
    for (std::list<Basic::Statement*>::const_iterator s = statements.begin(); s != statements.end(); s++) {
        (*s)->execute();
    }
}

void Basic::PrintStatement::execute (void) {
    for (std::list<Basic::Expression*>::const_iterator e = expressions.begin(); e != expressions.end(); e++) {
        (*e)->execute();
        fputs((*e)->getResult().getStringValue(), stdout);
    }
    if (append_eol)  putchar('\n');
}

void Basic::InputStatement::execute (void) {
    
}

void Basic::LetStatement::execute (void) {
    
}

void Basic::CallStatement::execute (void) {
    
}

void Basic::IfStatement::execute (void) {
    
}

void Basic::DoStatement::execute (void) {
    
}

void Basic::ForStatement::execute (void) {
    
}

void Basic::DimStatement::execute (void) {
    
}

void Basic::AcceptedParamList::execute (void) {
    
}

void Basic::ParamList::execute (void) {
    ;  // does nothing
}

void Basic::ArraySubscript::execute (void) {
    
}

void Basic::ArrayDimension::execute (void) {
    
}

void Basic::FunctionDefinition::execute (void) {
    
}

void Basic::SubDefinition::execute (void) {
    
}


Basic::PrintStatement::~PrintStatement() {
    for (std::list<Basic::Expression *>::iterator e = expressions.begin(); e != expressions.end(); e++) {
        delete (*e);
    }
}

Basic::IfStatement::~IfStatement() {
    for (std::list<ConditionalBlock>::iterator b = conditional_blocks.begin(); b != conditional_blocks.end(); b++) {
        delete b->condition;
        delete b->block;
    }
    
    if (else_block)  delete else_block;
}

Basic::ForStatement::~ForStatement() {
    delete start;
    delete end;
    if (step)  delete step;
    delete body;
}

Basic::DimStatement::~DimStatement() {
    for (std::list<Dimensionable>::iterator d = dimensionables.begin(); d != dimensionables.end(); d++) {
        if (d->second)  delete d->second;
    }
}

Basic::ArraySubscript::~ArraySubscript() {
    for (std::list<Basic::Expression *>::iterator e = expressions.begin(); e != expressions.end(); e++) {
        delete (*e);
    }
}

Basic::ArrayDimension::~ArrayDimension() {
    for (std::list<ArrayDimension::Specification>::iterator d = dimensions.begin(); d != dimensions.end(); d++) {
        delete d->first;
        if (d->second)  delete d->second;
    }
}

Basic::AcceptedParamList::~AcceptedParamList() {
    
}

Basic::ParamList::~ParamList() {
    for (std::vector<Expression *>::iterator e = expressions.begin(); e != expressions.end(); e++) {
        delete (*e);
    }
}

Basic::Block::~Block() {
    for (std::list<Basic::Statement *>::iterator s = statements.begin(); s != statements.end(); s++) {
        delete (*s);
    }
}

Basic::MultiplicativeExpression::~MultiplicativeExpression() {
    delete first_term;
    for (std::list<OperatorTerm>::iterator e = other_terms.begin(); e != other_terms.end(); e++) {
        delete (e->term);
    }
}

Basic::AdditiveExpression::~AdditiveExpression() {
    delete first_term;
    for (std::list<OperatorTerm>::iterator e = other_terms.begin(); e != other_terms.end(); e++) {
        delete (e->term);
    }
}


Basic::AndExpression::~AndExpression() {
    for (std::list<Basic::Expression *>::iterator e = terms.begin(); e != terms.end(); e++) {
        delete (*e);
    }
}

Basic::OrExpression::~OrExpression() {
    for (std::list<Basic::Expression *>::iterator e = terms.begin(); e != terms.end(); e++) {
        delete (*e);
    }    
}
