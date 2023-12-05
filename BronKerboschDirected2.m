function cliques = BronKerboschDirected2(matrix)
    cliques = {};
    tmp = matrix;
    matrix = zeros(size(matrix));
    matrix(tmp == 1 & tmp.'==1) = 1;
    function findCliques(R, P, X)
        if isempty(P) && isempty(X)
            cliques{end+1} = R;
        else
            for node = P
                neighbors = [];
                for i=1:size(matrix,1) 
                    if matrix(node,i)==1
                        neighbors(end + 1) = i;
                    end
                end
                findCliques(union(R, node), intersect(P, neighbors), intersect(X, neighbors));
                P = setdiff(P, node);
                X = union(X, node);
            end
        end
    end

    findCliques([], 1:size(matrix,1), [])
end