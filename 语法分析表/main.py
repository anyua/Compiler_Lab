import time
STATE_OFFSET = 10000
ACCEPT_STATE = 23333


def get_grammer(filename):
    with open(filename, 'r') as f:
        nonterminal_character = f.readline()
        terminals = f.readline()
        production_rules = f.readlines()
    v = nonterminal_character.split()
    t = terminals.split()
    s = v[0]
    p = []
    for i in production_rules:
        head, body = i.split("->", 1)
        head = head.strip()
        body = body.split()
        p.append((head, body))
    return v, t, p, s


def get_first(g):
    v, t, p, s = g
    first = {}
    link = [[0 for i in range(len(v))] for i in range(len(v))]
    for c in t:
        first[c] = {c}
    for c in v:
        first[c] = set()
        for rule in p:
            if c == rule[0] and '~' in rule[1]:
                first[c] = set('~')
                break
    for c in v:
        for rule in p:
            head = rule[0]
            body = rule[1]
            if c == head:
                for i in body:
                    if i in t:
                        first[c].add(i)
                        break
                    elif i in v and '~' not in first[i]:
                        link[v.index(c)][v.index(i)] = 1
                        break
    # print(link)
    while 1:
        flag = 1
        for i in range(len(v)):
            for j in range(len(v)):
                if link[i][j] and first[v[j]] - first[v[i]]:
                    first[v[i]] = first[v[i]].union(first[v[j]])
                    flag = 0
        if flag == 1:
            break
    return first


def get_follow(g, first):
    v, t, p, s = g
    v_t = v + t
    follow = {}
    for c in v:
        follow[c] = set()
    follow[s].add('$')
    follow_first_link = [[0 for i in range(len(v_t))] for i in range(len(v))]
    follow_follow_link = [[0 for i in range(len(v))] for i in range(len(v))]
    for rule in p:
        head = rule[0]
        body = rule[1]
        for i in range(len(body)):
            c = body[i]
            if c in v:
                next_c = 0
                if i + 1 < len(body):
                    next_c = body[i + 1]
                if next_c:
                    follow_first_link[v.index(c)][v_t.index(next_c)] = 1
                    for b in body[i + 1:]:
                        if '~' not in first[b]:
                            break
                    else:
                        follow_follow_link[v.index(c)][v.index(head)] = 1
                else:
                    follow_follow_link[v.index(c)][v.index(head)] = 1
    while 1:
        flag = 1
        for i in range(len(v)):
            for j in range(len(v_t)):
                no_psilon = first[v_t[j]] - {'~'}
                if follow_first_link[i][j] and no_psilon - follow[v[i]]:
                    follow[v[i]] = follow[v[i]].union(no_psilon)
                    flag = 0
        for i in range(len(v)):
            for j in range(len(v)):
                if follow_follow_link[i][j] and follow[v[j]] - follow[v[i]]:
                    follow[v[i]] = follow[v[i]].union(follow[v[j]])
                    flag = 0
        if flag == 1:
            break
    return follow


def get_closuer(g, item, first):
    v, t, p, s = g
    rule, dot, foward_c = item
    item_closuer = [item]
    added = []
    if dot < len(rule[1]):
        b = rule[1][dot]
        beta = rule[1][dot+1:]
    else:
        b = ''
    if b in v:
        added.append(b)
    while added:
        flag = 1
        for a in added:
            for r in p:
                if r[0] == a:
                    if (r, 0, []) not in item_closuer:
                        item_closuer.append((r, 0, []))
                    if r[1][0] in v and r[1][0] not in added:
                        added.append(r[1][0])
                        flag = 0
        if flag == 1:
            break
    return item_closuer


def get_closuer2(g, item, first):
    v, t, p, s = g
    item_closuer = item
    while 1:
        flag = 1
        for i in item_closuer:
            rule, dot, foward_c = i
            if dot < len(rule[1]):
                for new_r in [r for r in p if rule[1][dot] == r[0]]:
                    new_dot = 0
                    if new_r[1] == ['~']:
                        new_dot = 1
                    new_foward_c = set()
                    beta = rule[1][dot+1:]
                    for c in beta:
                        new_foward_c = new_foward_c.union(first[c])
                        if '~' not in first[c]:
                            break
                        else:
                            new_foward_c.remove('~')
                    else:
                        for b in foward_c:
                            if b=='$':
                                new_foward_c = new_foward_c.union({'$'})
                            else:
                                new_foward_c = new_foward_c.union(first[b])
                    new_item = (new_r, new_dot, new_foward_c)
                    if new_item not in item_closuer:
                        item_closuer.append(new_item)
                        flag = 0
        if flag:
            break
    return item_closuer


def get_goto(g, item, x, first):
    v, t, p, s = g
    goto_item = []
    for i in item:
        rule, dot, foward_c = i
        if dot<len(rule[1]) and rule[1][dot] == x:
            goto_item.append((rule, dot+1, foward_c))
    return get_closuer2(g, goto_item, first)


def items(g, first):
    v, t, p, s = g
    first_item = [(p[0], 0, {'$'})]
    c = [get_closuer2(g, first_item, first)]
    while 1:
        flag = 1
        for i in c:
            for x in v+t:
                new_item = get_goto(g, i, x, first)
                if new_item and new_item not in c:
                    c.append(new_item)
                    # print(len(c))
                    flag = 0
        if flag:
            break
    return c


def get_action_table(g, c, first):
    v, t, p, s = g
    t = t + ['$']
    action_table = [['-1' for i in range(len(t))] for i in range(len(c))]
    for ii in c:
        for item in ii:
            rule, dot, foward_c = item
            if rule[1] == ['~']:
                for i in t:
                    action_table[c.index(ii)][t.index(i)] = str(p.index(rule) + STATE_OFFSET)
            if dot < len(rule[1]):
                if rule[1][dot] in t:
                    a = rule[1][dot]
                    j = c.index(get_goto(g, ii, a, first))
                    action_table[c.index(ii)][t.index(a)] = str(j)
                    # action_table[c.index(ii)][t.index(a)] = "shift " + str(j)
            else:
                if rule[0] != s:
                    for a in foward_c:
                        action_table[c.index(ii)][t.index(a)] = str(p.index(rule)+STATE_OFFSET)
                        # action_table[c.index(ii)][t.index(a)] = "reduce " + str(p.index(rule))
                elif rule == p[0] and foward_c == {'$'}:
                    action_table[c.index(ii)][t.index('$')] = str(ACCEPT_STATE)
    return action_table


def get_goto_table(g, c, first):
    v, t, p, s = g
    goto_table = [['0' for i in range(len(v)-1)] for i in range(len(c))]
    for ii in c:
        for a in v:
            ij = get_goto(g, ii, a, first)
            if ij:
                goto_table[c.index(ii)][v.index(a)-1] = str(c.index(ij))
    return goto_table


if __name__ == "__main__":
    Filename = "TINY_C.cfg"
    Action_table_filename = "tiny_c_action.txt"
    Goto_table_filename = "tiny_c_goto.txt"
    # Filename = "CFG.txt"
    # Action_table_filename = "CFG_action.txt"
    # Goto_table_filename = "CFG_goto.txt"
    start_time = time.time()
    G = get_grammer(Filename)
    end_time = time.time()
    print("---------------------------------------------------")
    print("Grammer include complete by {0:.3f} seconds.".format(end_time-start_time))
    print(time.strftime("%Y-%m-%d %H:%M:%S", time.localtime()))
    print("Grammer:")
    for i in G:
        print(i)
    print("---------------------------------------------------")
    print("Get first set ...")
    start_time = time.time()
    First = get_first(G)
    end_time = time.time()
    print("---------------------------------------------------")
    print("Get first set complete by {0:.3f} seconds.".format(end_time-start_time))
    print(time.strftime("%Y-%m-%d %H:%M:%S", time.localtime()))
    print("First set:")
    for i in First.keys():
        print(str(i) + " = ", end="")
        print(First[i])
    print("---------------------------------------------------")
    print("Get follow set...")
    start_time = time.time()
    Follow = get_follow(G, First)
    end_time = time.time()
    print("---------------------------------------------------")
    print("Get follow set complete by {0:.3f} seconds.".format(end_time-start_time))
    print(time.strftime("%Y-%m-%d %H:%M:%S", time.localtime()))
    print("Follow set:")
    for i in Follow.keys():
        print(str(i) + " = ", end="")
        print(Follow[i])
    print("---------------------------------------------------")
    print("Get items set...")
    start_time = time.time()
    Items = items(G, First)
    end_time = time.time()
    print("---------------------------------------------------")
    print("Get items set complete by {0:.3f} seconds.".format(end_time-start_time))
    print(time.strftime("%Y-%m-%d %H:%M:%S", time.localtime()))
    print("collect " + str(len(Items)) + " items")
    # for i in Items:
    #     print(i)
    print("---------------------------------------------------")
    print("Get action table...")
    start_time = time.time()
    Action = get_action_table(G, Items, First)
    print("---------------------------------------------------")
    print("Get action table complete by {0:.3f} seconds.".format(end_time-start_time))
    print(time.strftime("%Y-%m-%d %H:%M:%S", time.localtime()))
    print("action:")
    # for i in Action:
    #    print(i)
    print("---------------------------------------------------")
    print("Get goto table ...")
    start_time = time.time()
    Goto = get_goto_table(G, Items, First)
    end_time = time.time()
    print("---------------------------------------------------")
    print("Get goto table complete by {0:.3f} seconds.".format(end_time-start_time))
    print(time.strftime("%Y-%m-%d %H:%M:%S", time.localtime()))
    print("goto: ")
    # for i in Goto:
    #    print(i)
    with open(Action_table_filename, 'w') as f:
        for i in Action:
            f.write('\t'.join(i) + '\n')
    with open(Goto_table_filename, 'w') as f:
        for i in Goto:
            f.write('\t'.join(i) + '\n')

