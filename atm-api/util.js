module.exports = {
    // Emulates the ?? (Nullish coalescing) operator (The operator is only available in node v14 and up)
    nshcl: (value, default_value) => {
        return value === null || value === undefined ? default_value : value;
    },

    handleQueryResult: (res, error, result) => {
        if(error) {
            res.json(error);
            res.status(500);
            return;
        }

        res.json(result);
    }
};
