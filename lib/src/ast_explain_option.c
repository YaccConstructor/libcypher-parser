/* vi:set ts=4 sw=4 expandtab:
 *
 * Copyright 2016, Chris Leishman (http://github.com/cleishm)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "../../config.h"
#include "astnode.h"
#include "operators.h"
#include "util.h"
#include <assert.h>


struct explain_option
{
    cypher_astnode_t _astnode;
};


static cypher_astnode_t *clone(const cypher_astnode_t *self);
static ssize_t detailstr(const cypher_astnode_t *self, char *str, size_t size);


static const struct cypher_astnode_vt *parents[] =
    { &cypher_statement_option_astnode_vt };

const struct cypher_astnode_vt cypher_explain_option_astnode_vt =
    { .parents = parents,
      .nparents = 1,
      .name = "EXPLAIN",
      .detailstr = detailstr,
      .release = cypher_astnode_release,
      .clone = clone };


cypher_astnode_t *cypher_ast_explain_option(struct cypher_input_range range)
{
    struct explain_option *node = calloc(1, sizeof(struct explain_option));
    if (node == NULL)
    {
        return NULL;
    }
    if (cypher_astnode_init(&(node->_astnode), CYPHER_AST_EXPLAIN_OPTION,
            NULL, 0, range))
    {
        free(node);
        return NULL;
    }
    return &(node->_astnode);
}


cypher_astnode_t *clone(const cypher_astnode_t *self)
{
    REQUIRE_TYPE(self, CYPHER_AST_EXPLAIN_OPTION, NULL);
    return cypher_ast_explain_option(self->range);
}


ssize_t detailstr(const cypher_astnode_t *self, char *str, size_t size)
{
    REQUIRE_TYPE(self, CYPHER_AST_EXPLAIN_OPTION, -1);
    return 0;
}
