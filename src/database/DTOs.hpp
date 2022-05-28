#pragma  once

#include "oatpp/web/client/ApiClient.hpp"
#include "oatpp/core/macro/codegen.hpp"


#include OATPP_CODEGEN_BEGIN(DTO)

class DBHighscoreElementDto : public oatpp::DTO {

    DTO_INIT(DBHighscoreElementDto, DTO)

    DTO_FIELD(Int32, id);

    DTO_FIELD(Int32, money);

    DTO_FIELD(String, name);

};

class DBHighscoreDto : public oatpp::DTO {

    DTO_INIT(DBHighscoreDto, DTO)

    DTO_FIELD(String, _id);

    DTO_FIELD(String, _rev);

    DTO_FIELD(Vector < Object < DBHighscoreElementDto > >, highscore);

};

#include OATPP_CODEGEN_END(DTO)
