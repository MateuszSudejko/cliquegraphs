function result = greedyRandom(matrix)
    max=0;
    R = [];
    P = 1:size(matrix,1);
    while ~isempty(P)
        R = []; % Reset R to empty for the next iteration
        N = P;
        while ~isempty(N)
            v_index = randi(length(P)); % Randomly select an index from P
            v = P(v_index); % Retrieve the value at the selected index
            neighbors = [];
            for i=1:size(matrix,1) 
                if matrix(v,i)==1
                    neighbors(end + 1) = i;
                end
            end
            N = intersect(N, neighbors); % Intersect N with the neighborhood of v
            R = [R, v]; % Add v to R
        end

        R=unique(R);
        
        if size(R,2)>max
            max=size(R,2);
        end
       
        % Remove elements in R from P
        P = setdiff(P, R);
    end
    
    result = R;
end
