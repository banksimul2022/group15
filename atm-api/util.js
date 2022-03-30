module.exports = {
    handleQueryResult: (res, error, result) => {
        if(error) {
            res.json(error);
            res.status(500);
            return;
        }

        res.json(result);
    }
};
