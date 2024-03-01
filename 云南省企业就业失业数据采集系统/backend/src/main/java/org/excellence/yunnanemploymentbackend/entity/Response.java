package org.excellence.yunnanemploymentbackend.entity;

import lombok.AllArgsConstructor;
import lombok.Data;

@AllArgsConstructor
@Data
public class Response<T> {
    private Boolean success;
    private String message;
    private T data;
}
