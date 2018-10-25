function [STEGO,embedding_efficiency,changes] = nsf5_simulation_cste_payload_color(COVER,changes_all,payload_chroma,SEED)

%%% load the cover image
STEGO=COVER;
for index_color=1:3
        if (index_color==1)
                changes=round(changes_all*(1-payload_chroma));
        else
                changes=round(changes_all*(payload_chroma/2));
        end
        DCT=COVER.coef_arrays{index_color};
        ALPHA=1;
        if ALPHA>0
            %%% embedding simulation
            nAC = nnz(DCT); % number of nonzero AC DCT coefficients
            nzAC = nnz(DCT)-nnz(DCT(1:8:end,1:8:end)); % number of nonzero AC DCT coefficients
            ALPHA=changes/nzAC;
            embedding_efficiency = ALPHA/invH(ALPHA); % bound on embedding efficiency
            changes = ceil(changes/embedding_efficiency); % number of changes nsF5 would make on bound
            changeable = (DCT~=0); % mask of all nonzero DCT coefficients in the image
            changeable(1:8:end,1:8:end) = false; % do not embed into DC modes
            changeable = find(changeable); % indexes of the changeable coefficients
            rand('state',SEED); % initialize PRNG using given SEED
            changeable = changeable(randperm(nzAC)); % create a pseudorandom walk over nonzero AC coefficients
            to_be_changed = changeable(1:changes); % coefficients to be changed
            DCT(to_be_changed) = DCT(to_be_changed)-sign(DCT(to_be_changed)); % decrease the absolute value of the coefficients to be changed
        end
        STEGO.coef_arrays{index_color}=DCT;

end

function res = invH(y)
% inverse of the binary entropy function
to_minimize = @(x) (H(x)-y)^2;
res = fminbnd(to_minimize,eps,0.5-eps);

function res = H(x)
% binary entropy function
res = -x*log2(x)-(1-x)*log2(1-x);
