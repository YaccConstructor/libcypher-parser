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
#include "util.h"
#include <assert.h> 

struct path_pattern
{
    cypher_astnode_t _astnode;
    const cypher_astnode_t *identifier;
    enum cypher_rel_direction direction;
    //Need path alternatives and repetitions
};

static cypher_astnode_t *clone(const cypher_astnode_t *self,
        cypher_astnode_t **children);
static ssize_t detailstr(const cypher_astnode_t *self, char *str, size_t size);

const struct cypher_astnode_vt cypher_path_pattern_astnode_vt =
    { .name = "path pattern",
      .detailstr = detailstr,
      .release = cypher_astnode_release,
      .clone = clone };
      
cypher_astnode_t *cypher_ast_path_pattern(const cypher_astnode_t *identifier,
        enum cypher_rel_direction direction, cypher_astnode_t **children, unsigned int nchildren,
        struct cypher_input_range range)
{
    struct path_pattern *node = calloc(1, sizeof(struct path_pattern));
    if (node == NULL)
    {
        return NULL;
    }
    if (cypher_astnode_init(&(node->_astnode), CYPHER_AST_PATH_PATTERN,
            children, nchildren, range))
    {
        goto cleanup;
    }
    node->identifier = identifier;
    node->direction = direction;
    return &(node->_astnode);

    int errsv;
cleanup:
    errsv = errno;
    free(node);
    errno = errsv;
    return NULL;
}

cypher_astnode_t *clone(const cypher_astnode_t *self,
        cypher_astnode_t **children)
{
    /**REQUIRE_TYPE(self, CYPHER_AST_PATH_PATTERN, NULL);*/
    struct path_pattern *node = container_of(self, struct path_pattern, _astnode);

    cypher_astnode_t *clone = cypher_ast_path_pattern(node->identifier,
            node->direction, children, self->nchildren,
            self->range);
    return clone;
}

const cypher_astnode_t *cypher_ast_path_pattern_get_identifier(
        const cypher_astnode_t *astnode)
{
    struct path_pattern *node =
            container_of(astnode, struct path_pattern, _astnode);
    return node->identifier;
}

enum cypher_rel_direction cypher_ast_path_pattern_get_direction(
        const cypher_astnode_t *astnode)
{
    struct path_pattern *node =
            container_of(astnode, struct path_pattern, _astnode);
    return node->direction;
}

ssize_t detailstr(const cypher_astnode_t *self, char *str, size_t size)
{
    struct path_pattern *node = container_of(self, struct path_pattern, _astnode);
    
    size_t n = 0;
    ssize_t r = snprintf(str, size, "%s-/",
            (node->direction == CYPHER_REL_INBOUND)? "<" : "");
    if (r < 0)
    {
        return -1;
    }
    n += r;

    if (node->identifier != NULL)
    {
        r = snprintf(str+n, (n < size)? size-n : 0, "@%u",
                node->identifier->ordinal);
        if (r < 0)
        {
            return -1;
        }
        n += r;
    }
    
    r = snprintf(str+n, (n < size)? size-n : 0, "/-%s",
            (node->direction == CYPHER_REL_OUTBOUND)? ">" : "");
    if (r < 0)
    {
        return -1;
    }
    n += r;

    return n;
}