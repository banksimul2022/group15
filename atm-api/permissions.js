const util = require("util");

// Tells the permission builder to fill all child objects with keys from this array
const FILL_KEY = "__fill";

//=========================\\

const permissions = {
    crud: {
        [FILL_KEY]: ["customer", "account", "card", "transaction", "customer_has_account"],
        read: {},
        write: {},
        all: {}
    }
};

//=========================\\

let flagIndex = 1;
let nodes = [ permissions ];

const getNextBit = () => {
    return 1 << (flagIndex++ - 1);
};

while(nodes.length) {
    const node = nodes.pop();
    const fill = FILL_KEY in node ? node[FILL_KEY] : null;

    if(fill !== null && !Array.isArray(fill)) {
        throw util.format("'%s' must be an array but it was '%s'!", FILL_KEY, typeof(fill));
    }

    for(let key in node) {
        if(key === FILL_KEY) continue;

        if(node[key] === null) {
            node[key] = getNextBit();
            continue;
        }

        if(typeof(node[key]) !== "object" || Array.isArray(node[key])) {
            throw util.format("Unsupported type '%s' in permissions object!", typeof(node[key]));
        }

        if(fill === null) {
            nodes.push(node[key]);
            continue;
        }

        for(let i = 0; i < fill.length; i++) {
            node[key][fill[i]] = getNextBit();
        }
    }

    if(fill !== null) {
        delete node[FILL_KEY];
    }
}

module.exports = Object.freeze(permissions);
