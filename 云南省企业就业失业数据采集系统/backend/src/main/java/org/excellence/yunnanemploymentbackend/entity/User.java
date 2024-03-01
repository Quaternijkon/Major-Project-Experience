package org.excellence.yunnanemploymentbackend.entity;

import jakarta.persistence.Entity;
import jakarta.persistence.Id;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@NoArgsConstructor
@Entity
public class User {
    public static class UserType {
        static public final Integer ENTERPRISE = 0;
        static public final Integer CITY = 1;
        static public final Integer PROVINCE = 2;

    }
    @Id private String userId;
    private String password;
    private Integer userType;//
}
